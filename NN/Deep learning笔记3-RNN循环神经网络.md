
---
title: Deep learning笔记3-RNN循环神经网络
lang: zh
date: 2017-10-18 09:18:56
---

### 1. 循环神经网络（RNN）
-------------------------------------

<center>![CNN](/image/DL/2/1.png)</center> 

-------------------------------------

一个循环神经网络层组成。

#### 1.1. 循环神经网络输出值的计算


-------------------------------------
  
推荐阅读 - [Gluon ](https://zh.gluon.ai/cnn-scratch.html "Title") 

##### 1.1.1. 循环层输出值的计算

图像大小、步幅和循环后的Feature Map大小，满足下面的关系：

<center>![CNN](/image/DL/2/4.gif)</center>  

##### 1.1.2. 池化层输出值的计算



#### 1.2. 循环公式的计算

用

#### 1.3. 循环神经网络的训练

先前向传播，再反向传播，利用链式求导计算损失函数对每个权重的偏导数（梯度），然后再根据梯度下降公式更新权重w



### 2. 基于TensorFlow的实现（RNN/LSTM with TF）

#### 2.1. TF循环神经网络的基本实现

##### 2.1.1. 循环层

```python

# Input/Image
input = tf.placeholder(
    tf.float32,
    shape=[None, image_height, image_width, color_channels])

# Weight and bias
weight = tf.Variable(tf.truncated_normal(
    [filter_size_height, filter_size_width, color_channels, k_output]))
bias = tf.Variable(tf.zeros(k_output))

# Apply Convolution
conv_layer = tf.nn.conv2d(input, weight, strides=[1, 2, 2, 1], padding='SAME')
# Add bias
conv_layer = tf.nn.bias_add(conv_layer, bias)
# Apply activation function
conv_layer = tf.nn.relu(conv_layer)

```

weights 作为滤波器，[1, 2, 2, 1] 作为 strides。TensorFlow 对每一个 input 维度使用一个单独的 stride 参数，[batch, input_height, input_width, input_channels]。通常把 batch 和 input_channels （strides 序列中的第一个第四个）的 stride 设为 1。


#### 2.2. TF循环神经网络的实现例

Make TV scripts using RNN(Recurrent Neural Networks) and LSTM(Long Short-Term Memory) network. Generate a new TV script with the model from TV scripts training data sets.
RNN(Recurrent Neural Networks)及びLSTM(Long Short-Term Memory)を使用して、訓練データを用いたモデルを作成して、新しいテレビスクリプトを生成する。

程序实例 - [Github Link](https://github.com/HJTSO/tv-script-generation/blob/master/dlnd_tv_script_generation.ipynb "Title") 


### 3. 词分析框架

#### 3.1. word2vec
#### 3.2. seg2seq

推荐阅读 - [LSTMで夏目漱石ぽい文章の生成](https://qiita.com/elm200/items/6f84d3a42eebe6c47caa "Title")

-------------------------------------

### 程序实例（Program Example）

- [Github Link](https://github.com/HJTSO/tv-script-generation/blob/master/dlnd_tv_script_generation.ipynb "Title") 

### 参考资料（Reference）

- [Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/ "Title") 

- [TensorFlow中RNN实现](https://zhuanlan.zhihu.com/p/28196873 "Title") 

- [零基础入门深度学习(5) - 循环神经网络](https://zybuluo.com/hanbingtao/note/541458 "Title") 

- [零基础入门深度学习(6) - 长短时记忆网络](https://zybuluo.com/hanbingtao/note/581764 "Title") 

- [LSTMネットワークの概要](https://qiita.com/KojiOhki/items/89cd7b69a8a6239d67ca "Title") 

- [わかるLSTM](https://qiita.com/t_Signull/items/21b82be280b46f467d1b "Title") 

