# 关于如何部署开发服务器（Linux服务器使用经验）

### 从运营商（腾讯云，阿里云等）获取服务器，并可以登录后

1. 检查版本 uname -r  
2. 更新yum包 sudo yum -y update  
3. 设置yum源  
（中央仓库）yum-config-manager --add-repo http://download.docker.com/linux/centos/docker-ce.repo  
（阿里仓库）yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo  


### 防火墙设置
1. 启动和关闭  
systemctl start firewalld  
systemctl stop firewalld  
2. 选择端口打开 firewall-cmd --zone=public --add-port=1935/tcp --permanent  
3. 端口相关  
查看对应端口占用 netstat –ntlp  
查看所有已打卡的端口 netstat -ntulp | grep 7777  

### 其它常用命令
1. 开放文件权限 sudo chmod 755 /var/***
2. 常用端口:  
window远程 3389,frp 7050 7080,p41666,httpd 8090,redis 6379,mysql 3306,DS 7777,WebSocket 10086  


Ps: 如果是轻量应用服务器，只是通过centos命令开启防火墙还不够，一定要记得在服务商管理页面的防火墙一栏中设置防火墙规则！  


### 安装docker
1. 先试着删除旧版（如果之前安装过的话） yum remove docker docker-common docker-selinux docker-engine  
2. 安装需要的软件包， yum-util 提供yum-config-manager功能，另两个是devicemapper驱动依赖  
yum install -y yum-utils device-mapper-persistent-data lvm2  
3. 选择docker版本并安装（需要先设置源）  
查看可用版本有哪些 yum list docker-ce --showduplicates | sort –r  
选择一个版本安装   yum -y install docker-ce-18.06.3.ce（版本号）  
4. 设置开机启动  
systemctl start docker  
systemctl enable docker  


### docker部署redis
1. 创建配置文件 sudo mkdir /usr/local/apps/redis/redis.conf  
2. 通过docker执行命令  
sudo docker run -p 6379:6379 --name redis -v /usr/local/apps/redis/redis.conf:/etc/redis/redis.conf -v /usr/local/apps/redis/data:/data -d redis:5.0 redis-server /etc/redis/redis.conf --appendonly yes  

Ps:  
1. 出现memoryovercommit报错（内存太小引起报警）  
sudo bash -c "echo 1 > /proc/sys/vm/overcommit_memory"  
2. 镜像版本太新导致运行失败，更好镜像版本，比如上述用的是5.0  
3. 一切正常但还是连不上：查看防火墙一栏内容，是否正确设置  


### 部署httpd
### 部署frp