import os
import sys
import subprocess
import argparse
import jinja2

# 引擎目录
engine_dir = 'D:/53/UnrealEngine-ToonLit/UnrealEngine/Engine'

TEMPL = """

# 签出所有可能影响到的文件
{{P4CMD}} edit {{CWD}}/Private/*.*
{{P4CMD}} edit {{CWD}}/Public/*.*

{{RPC}} -dllexport_decl=MRPC_API -src_file=GameRpc.rpcdef -hpp_dst_dir={{CWD}}/Public/ -cpp_dst_dir={{CWD}}/Private/

# 还原未修改的文件
{{P4CMD}} revert -a {{CWD}}/Private/...
{{P4CMD}} revert -a {{CWD}}/Public/...

"""


def get_ue_platform_name():
    platform = sys.platform
    if platform == 'darwin':
        return 'Mac'
    elif platform == 'win32':
        return 'Win64'
    else:
        return 'Linux'


def main():
    platform = get_ue_platform_name()
    if platform == 'Win64':
        py_cmd = f'{engine_dir}/Binaries/ThirdParty/Python3/{platform}/python.exe'
        p4_cmd = f'{engine_dir}/Binaries/ThirdParty/Perforce/{platform}/p4.exe'
    else:
        py_cmd = f'{engine_dir}/Binaries/ThirdParty/Python3/{platform}/bin/python'
        p4_cmd = f'{engine_dir}/Binaries/ThirdParty/Perforce/{platform}/p4'
    cwd = os.getcwd()
    p4_cmd = os.path.abspath(cwd + '/' + p4_cmd)
    py_cmd = os.path.abspath(cwd + '/' + py_cmd)

    meta_data = {
        "CWD": cwd,
        "P4CMD": p4_cmd,
        "PYCMD": py_cmd,
        'RPC': f'{py_cmd} rpc_tool.py',
    }

    try:
        content = jinja2.Template(TEMPL).render(meta_data)
    except Exception as render_err:
        raise Exception(f'TEMPL 文件渲染失败\n{render_err}')

    my_env = os.environ.copy()
    my_env['P4CONFIG'] = '.p4config'
    for line in content.splitlines():
        line = line.strip().strip('\n').strip().strip('\n')
        if not line.startswith('#') and len(line) > 0:
            try:
                ret = subprocess.run(line.split(' '), env=my_env)
                if ret.returncode != 0:
                    raise Exception(f'code={ret.returncode}')
            except Exception as run_err:
                raise Exception(f'指令执行失败\ncmd={line}\n{run_err} ')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-log_file', nargs='?', const='none', help='日志文件')
    args = parser.parse_args()

    try:
        main()
    except Exception as e:
        text = f'ERROR {e}'
        if args.log_file is not None:
            with open(args.log_file, 'wb') as f:
                f.write(text.encode('utf-8'))
        print(text)
        exit(1)
    print(f'DONE.')
    exit(0)
