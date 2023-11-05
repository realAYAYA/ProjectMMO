#!/usr/bin/env python3
# -*- mode: python; coding: utf-8-unix; -*-
# coding: utf-8

import subprocess
import argparse
import os
import sys
import copy
import pathlib
import collections
import jinja2


STUB_HPP_TEMPL = """#pragma once
#include "CoreMinimal.h"

#include "NetFwd.h"
#include "GameMessage.h"
{% for inc in include_list %}
#include "{{inc}}"
{%- endfor %}

#include "{{service_name}}Client.generated.h"

class FClientRpcManager;

{% for rpc_entry in rpc_list %}
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOn{{rpc_entry['name']}}Result, ERpcErrorCode, InErrorCode, F{{rpc_entry['p2']}}, InData);
{% endfor %}

{% for notify_entry in notify_list %}
{%- if notify_entry["type"] == 2 %}
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn{{notify_entry['name']}}Result, F{{notify_entry['name']}}, InData);
{%- endif %}
{% endfor %}

UCLASS(BlueprintType, Blueprintable)
class {{dllexport_decl}} U{{service_name}}Client : public UObject
{
    GENERATED_BODY()

public:

    void Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn);

    {%- for rpc_entry in rpc_list %}

    /**
    {%- for comment in rpc_entry['comment'] %}
     * {{comment}}
    {%- endfor %}
    */
    UFUNCTION(BlueprintCallable, Category = "PorjectM", DisplayName="{{rpc_entry['name']}}")
    void K2_{{rpc_entry['name']}}(const F{{rpc_entry['p1']}}& InParams, const FOn{{rpc_entry['name']}}Result& InCallback);
    
    typedef TFunction<void(ERpcErrorCode, const F{{rpc_entry['p2']}}&)> FOn{{rpc_entry['name']}}ResultFunction;
    void {{rpc_entry['name']}}(const F{{rpc_entry['p1']}}& InReqMessage, const FOn{{rpc_entry['name']}}ResultFunction& InCallback) const;
    
    {%- endfor %}

    {% for notify_entry in notify_list %}
    /**
    {%- for comment in notify_entry['comment'] %}
     * {{comment}}
    {%- endfor %}
    */      
    {%- if notify_entry["type"] == 1 %}
    // Todo 来自服务器消息通知
    {%- else %}
    UPROPERTY(BlueprintAssignable, Category = "ProjectM") 
    FOn{{notify_entry['name']}}Result On{{notify_entry['name']}};
    {%- endif %}
    {% endfor %}
     
private:

    FClientRpcManager* Manager = nullptr;

    FConnectionPtr Connection;
};

"""

STUB_CPP_TEMPL = """
#include "{{service_name}}Client.h"
#include "RpcManager.h"

void U{{service_name}}Client::Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn)
{
    Manager = InManager;
    Connection = InConn;

    if (Manager)
    {    
        {%- for notify_entry in notify_list %}        
        {%- if notify_entry["type"] == 2 %}
        Manager->DispatchNotify([this](const FZPbConnectionPtr& InConn, const TSharedPtr<{{rpc_space_name}}::{{notify_entry['name']}}>& InMessage)
        {
            if (On{{notify_entry['name']}}.IsBound())
            {
                FZ{{notify_entry['name']}} Result = *InMessage;
                On{{notify_entry['name']}}.Broadcast(Result);
            }
        });
        {%- endif %}
        {%- endfor %}
    }
}

{% for rpc_entry in rpc_list %}
void U{{service_name}}Client::K2_{{rpc_entry['name']}}(const F{{rpc_entry['p1']}}& InParams, const FOn{{rpc_entry['name']}}Result& InCallback)
{
    if (!Manager || !Connection)
        return;

    {{rpc_entry['name']}}(InParams, [InCallback](const ERpcErrorCode ErrorCode, const F{{rpc_entry['p2']}}& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
        else
		{
			UE_LOG(LogGameNetwork, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
		}
    });
}

void U{{service_name}}Client::{{rpc_entry['name']}}(const F{{rpc_entry['p1']}}& InReqMessage, const FOn{{rpc_entry['name']}}ResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const F{{rpc_entry['p2']}} RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}

{% endfor %}

"""

#  ----------------------------------------------------------------------------
#  ----------------------------------------------------------------------------

INTERFACE_HPP_TEMPL = """#pragma once
#include "GameMessage.h"
{% for inc in include_list %}
#include "{{inc}}"
{%- endfor %}

class FServerRpcManager;

class {{dllexport_decl}} F{{service_name}}Interface
{

public:

    const TCHAR* GetName() const { return TEXT("{{service_name}}"); }

    explicit F{{service_name}}Interface(FServerRpcManager* InManager);

    virtual ~F{{service_name}}Interface() {}
    
    {% for rpc_entry in rpc_list %}
    /**
    {%- for comment in rpc_entry['comment'] %}
     * {{comment}}
    {%- endfor %}
    */
    typedef TFunction<void(const F{{rpc_entry['name']}}Req&, F{{rpc_entry['name']}}Ack&)> F{{rpc_entry['name']}}Callback;
    F{{rpc_entry['name']}}Callback On{{rpc_entry['name']}};
    {% endfor %}

};

"""

INTERFACE_CPP_TEMPL = """#include "{{service_name}}Interface.h"
#include "RpcManager.h"

F{{service_name}}Interface::F{{service_name}}Interface(FServerRpcManager* InManager)
{
{%- for rpc_entry in rpc_list %}
    InManager->AddMethod(F{{rpc_entry['p1']}}::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const F{{rpc_entry['p1']}} ReqMessage;
        F{{rpc_entry['p2']}} RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (On{{rpc_entry['name']}})
        {
            On{{rpc_entry['name']}}(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });    
{%- endfor %}
}

"""


def record_type(types, s):
    s = s.strip('::')
    if s == 'EMPTY':
        return
    
    space_name = ''
    type_name = ''
    data = s.split('::')    
    if len(data) >= 2:
        space_name = data[0]
        type_name = data[1]
    elif len(data) == 1:
        type_name = data[0]
    types[s] = {'space_name': space_name, 'type_name': type_name, }


def main(src_file, hpp_dst_dir, cpp_dst_dir, dllexport_decl):
    if hpp_dst_dir[-1] != '/':
        hpp_dst_dir += '/'

    if cpp_dst_dir[-1] != '/':
        cpp_dst_dir += '/'
    
    src_file_path = pathlib.Path(src_file)

    service_name = src_file_path.stem
    
    rpcs = list()
    notifys = list()
    includes = list()
    type_dict = dict()
    rpc_space_name = ''

    last_comment = list()
    with open(src_file_path, 'r', encoding='utf-8') as f:
        line_num = 0
        for line in f.readlines():
            line_num += 1
            line = line.strip().strip('\n').strip('\r')
            line = ' '.join(line.split())
            if line.startswith('#'):  # 以#号开头是注释
                last_comment.append(line.lstrip('#').strip())
                continue
            data = line.split(' ')
            op = data[0]
            if op == 'rpc':  # rpc函数
                if len(data) >= 4:
                    rpc = dict()
                    rpc['name'] = data[1]  # rpc函数名称
                    rpc['p1'] = 'EMPTY'  # 请求参数
                    rpc['p2'] = 'EMPTY'  # 返回参数
                    rpc['p1'] = data[2]
                    rpc['p2'] = data[3]
                    record_type(type_dict, data[2])
                    record_type(type_dict, data[3])
                    rpc['comment'] = copy.copy(last_comment)  # 注释
                    rpcs.append(rpc)
                else:
                    raise Exception(f'rpc格式错误，缺少必要字段！ 行号:{line_num}')
            elif op == 'send' or op == 'result':
                if len(data) >= 2:
                    notify = dict()
                    notify['name'] = data[1]
                    record_type(type_dict, data[1])
                    notify['comment'] = copy.copy(last_comment)  # 注释
                    if op == 'send':
                        notify['type'] = 1
                    else:
                        notify['type'] = 2
                    notifys.append(notify)
                else:
                    raise Exception(f'rpc格式错误，缺少必要字段！ 行号:{line_num}')
            elif op == 'include':  # include文件列表
                for e in data[1:]:
                    includes.append(e)
            elif op == "namespace":  # 名字空间
                rpc_space_name = data[1]
            last_comment.clear()
                
    type_list = list()
    for k in type_dict:
        type_list.append(type_dict[k])
    
    meta_data = {
        'service_name': service_name,
        'rpc_space_name': rpc_space_name,
        'include_list': includes,
        'rpc_list': rpcs,
        'notify_list': notifys,
        'type_list': type_list,
        'dllexport_decl': dllexport_decl,
    }

    try:
        stub_hpp_content = jinja2.Template(STUB_HPP_TEMPL).render(meta_data)
    except Exception as e:
        raise Exception('STUB_HPP_TEMPL 文件渲染失败 {}'.format(e))

    try:
        stub_cpp_content = jinja2.Template(STUB_CPP_TEMPL).render(meta_data)
    except Exception as e:
        raise Exception('STUB_CPP_TEMPL 文件渲染失败 {}'.format(e))
    
    try:
        interface_hpp_content = jinja2.Template(INTERFACE_HPP_TEMPL).render(meta_data)
    except Exception as e:
        raise Exception('INTERFACE_HPP_TEMPL 文件渲染失败 {}'.format(e))

    try:
        interface_cpp_content = jinja2.Template(INTERFACE_CPP_TEMPL).render(meta_data)
    except Exception as e:
        raise Exception('INTERFACE_CPP_TEMPL 文件渲染失败 {}'.format(e))
    
    stub_hpp_file_path = hpp_dst_dir + service_name + 'Client.h'
    stub_cpp_file_path = cpp_dst_dir + service_name + 'Client.cpp'
    
    interface_hpp_file_path = hpp_dst_dir + service_name + 'Interface.h'
    interface_cpp_file_path = cpp_dst_dir + service_name + 'Interface.cpp'
    
    # 写入磁盘
    try:
        with open(stub_hpp_file_path, 'wb') as f:
            f.write(stub_hpp_content.encode('utf-8'))
        with open(stub_cpp_file_path, 'wb') as f:
            f.write(stub_cpp_content.encode('utf-8'))
        with open(interface_hpp_file_path, 'wb') as f:
            f.write(interface_hpp_content.encode('utf-8'))
        with open(interface_cpp_file_path, 'wb') as f:
            f.write(interface_cpp_content.encode('utf-8'))
    except Exception as e:
        raise Exception('文件写入失败 {}'.format(e))    
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-src_file', nargs='?', const='none', help='源文件')
    parser.add_argument('-hpp_dst_dir', nargs='?', const='none', help='头文件目标目录')
    parser.add_argument('-cpp_dst_dir', nargs='?', const='none', help='源文件文件目标目录')
    parser.add_argument('-dllexport_decl', nargs='?', const='none', help='DLL导出宏')
    args = parser.parse_args()

    if args.src_file is None:
        print('ERROR: 未指定源文件')
        exit(1)

    if not os.path.exists(args.src_file):
        print('ERROR: 源文件非法! {}'.format(args.src_file))
        exit(1)

    if not os.path.exists(args.hpp_dst_dir):
        print('ERROR: 头文件输出目录非法! {}'.format(args.hpp_dst_dir))
        exit(1)

    if not os.path.exists(args.cpp_dst_dir):
        print('ERROR: 源文件输出目录非法! {}'.format(args.cpp_dst_dir))
        exit(1)

    dllexport_decl = args.dllexport_decl or ''

    try:
        main(args.src_file, args.hpp_dst_dir, args.cpp_dst_dir, dllexport_decl)
    except Exception as e:
        print('处理失败({}): {}'.format(args.src_file, e))
        exit(1)
    exit(0)
