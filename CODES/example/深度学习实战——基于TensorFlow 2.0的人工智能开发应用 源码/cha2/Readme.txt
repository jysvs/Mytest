# ����
- Ubuntu
- Python3.6
- Tensorflow2.0
# ���
- OpenCV
- Pillow
- Matplotlib
- Flask
- xlrd, xlwt
# Usage
## �����
���Ѿ�������Դ˲���
- �½�Anaconda���⻷��tf2
tf2Ϊ�½������⻷�����ƣ�������ʹ�ã������������Լ���ʵ������޸ġ�
```
conda create -n tf2 python=3.6
```
- ����tf2
```
conda activate tf2
```
- ��װTensorflow2.0
```
pip install tensorflow=2.0
```
## ִ�г���
���ж�Ӧ�ļ�����py�ļ���ʵ�ֶ�Ӧ�Ĺ��ܣ��μ�����������
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