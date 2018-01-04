# mumu
目前只支持vs2013，如果ide是vs2013那么请往下看。    
1.克隆代码之后，在代码层中找到bin文件夹。    
2.注册bin文件夹下面的mumu_addin.dll，windows下面需要管理员权限的cmd窗口，命令行如下：    
  regsvr32 mumu_addin.dll    
3.注册好之后重启vs，在工具->外接程序管理，启用mumu_addin.    
4.在代码中包含mu_object头文件就可以正常使用了，具体用法参照[信号与槽连接的使用](信号与槽连接的使用)。    
