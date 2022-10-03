# 环境
- Ubuntu
- Python3.6
- Tensorflow2.0
# 插件
- OpenCV
- Pillow
- Matplotlib
- Flask
- xlrd, xlwt
# Usage
## 搭建环境
若已经搭建，忽略此步。
- 新建Anaconda虚拟环境tf2
tf2为新建的虚拟环境名称，供测试使用，开发者依据自己的实际情况修改。
```
conda create -n tf2 python=3.6
```
- 激活tf2
```
conda activate tf2
```
- 安装Tensorflow2.0
```
pip install tensorflow=2.0
```
## 执行程序
运行对应文件名的py文件，实现对应的功能，参见书中描述。
```
python3.6 filename.py
```
TensorFlow 2.0 Upgrade Script
-----------------------------
Converted 1 files
Detected 0 issues that require attention
--------------------------------------------------------------------------------
================================================================================
Detailed log follows:

================================================================================
--------------------------------------------------------------------------------
Processing file 'tensorflow_test.py'
 outputting to 'tensorflow_testv2.0.py'
--------------------------------------------------------------------------------

9:9: INFO: Renamed 'tf.Session' to 'tf.compat.v1.Session'
19:9: INFO: Renamed 'tf.Session' to 'tf.compat.v1.Session'
--------------------------------------------------------------------------------