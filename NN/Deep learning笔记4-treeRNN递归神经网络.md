
---
title: Deep learning笔记4-RNN递归神经网络
lang: zh
date: 2017-10-19 18:18:56
---

### 1. 卷积神经网络简介（CNN）
-------------------------------------

<center>![CNN](/image/DL/2/1.png)</center> 

-------------------------------------

一个卷积神经网络（Convolutional Neural Network）由若干卷积层、池化层、全连接层组成。

#### 1.1. 卷积神经网络输出值的计算

以一个5*5的图像，使用一个3*3的filter进行卷积，得到一个3*3的Feature Map为例：

<center>![CNN](/image/DL/2/2.png)</center>

-------------------------------------

以步幅(stride)为1，依次计算出Feature Map中所有元素的值，计算过程：

<center>![CNN](/image/DL/2/3.gif)</center>  

-------------------------------------

用例来自 - [零基础入门深度学习(4) - 卷积神经网络](https://www.zybuluo.com/hanbingtao/note/485480 "Title")   

推荐阅读 - [Gluon - 卷积神经网络](https://zh.gluon.ai/cnn-scratch.html "Title") 

##### 1.1.1. 卷积层输出值的计算

图像大小、步幅和卷积后的Feature Map大小，满足下面的关系：

\begin{align}
W_2 &= (W_1 - F + 2P)/S + 1\qquad\\
H_2 &= (H_1 - F + 2P)/S + 1\qquad
\end{align}

在上面两个公式中：W2是卷积后Feature Map的宽度；W1是卷积前图像的宽度；F是filter的宽度；P是Zero Padding数量，Zero Padding是指在原始图像周围补几圈0，如果P的值是1，那么就补1圈0；S是步幅；H2是卷积后Feature Map的高度；H1是卷积前图像的宽度。

下面的显示包含两个filter的卷积层的计算。可以看到7*7*3输入，经过两个3*3*3filter的卷积(步幅为2)，得到了3*3*2的输出。另外也会看到下图的Zero padding是1，也就是在输入元素的周围补了一圈0。

<center>![CNN](/image/DL/2/4.gif)</center>  

##### 1.1.2. 池化层输出值的计算

池化层主要的作用是下采样，通过去掉Feature Map中不重要的样本，进一步减少参数数量。池化的方法最常用的是Max Pooling，即样本中取最大值，作为采样后的样本值。下例是2*2 max pooling：

<center>![CNN](/image/DL/2/5.png)</center>  

-------------------------------------

还有Mean Pooling，即取各样本的平均值。

#### 1.2. 卷积公式的计算

用X[i,j]表示图像的第i行第j列元素；对filter的每个权重进行编号，用W[m,n]表示第m行第n列权重，用Wb表示filter的偏置项；对Feature Map的每个元素进行编号，a[i,j]用表示Feature Map的第i行第j列元素；用f表示激活函数。

$$a_{i,j}=f(\sum_{m=0}^{2}\sum_{n=0}^{2}w_{m,n}x_{i+m,j+n}+w_b)$$

深度大于1的卷积计算公式：  

$$a_{i,j}=f(\sum_{d=0}^{D-1}\sum_{m=0}^{F-1}\sum_{n=0}^{F-1}w_{d,m,n}x_{d,i+m,j+n}+w_b)$$

#### 1.3. 卷积神经网络的训练

先前向传播，再反向传播，利用链式求导计算损失函数对每个权重的偏导数（梯度），然后再根据梯度下降公式更新权重w

#### 1.4. 关于权重w与偏置项b的初始化

##### 1.4.1. 权重w的初始化

① 均匀分布：tf.random_uniform()

② 正太分布：tf.random_normal()

③ 从截断的正态取值--横轴区间（μ-2σ，μ+2σ）95%面积：tf.truncated_normal()

④ 还有一个经验公式：从[-y, y]取值，其中 $y=1/\sqrt{n}$

根据经验使用③效果较佳…

##### 1.4.2. 偏置项b的初始化

一般使用tf.zeros()来初始化为零值. 

阅读参考 - [Role of Bias in Neural Networks](https://stackoverflow.com/questions/2480650/role-of-bias-in-neural-networks "Title")

“a bias value allows you to shift the activation function to the left or right”

### 2. 基于TensorFlow的实现（CNN with TF）

#### 2.1. TF卷积神经网络的基本实现

##### 2.1.1. 卷积层

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

##### 2.1.2. 池化层

```python

# Apply Max Pooling
conv_layer = tf.nn.max_pool(
    conv_layer,
    ksize=[1, 2, 2, 1],
    strides=[1, 2, 2, 1],
    padding='SAME')
    
```

tf.nn.max_pool() 函数实现最大池化时， ksize参数是滤波器大小，strides参数是步长。2x2 的滤波器配合 2x2 的步长是常用设定。

ksize 和 strides 参数也被构建为四个元素的列表，每个元素对应 input tensor 的一个维度 ([batch, height, width, channels])，对 ksize 和 strides 来说，batch 和 channel 通常都设置成 1。

#### 2.2. TF卷积神经网络的实现例

对 [CIFAR-10 数据集](https://www.cs.toronto.edu/~kriz/cifar.html "Title") 中的图片进行分类。
该数据集包含飞机、猫狗和其他物体。先预处理这些图片，然后用所有样本训练一个卷积神经网络。图片需要标准化（normalized），标签需要采用 one-hot 编码。构建卷积（convolution）、最大池化（max pooling）、丢弃（dropout）和完全连接（fully connected）的层。最后在样本图片上看到神经网络的预测结果。

具体实现不赘述，过程直接看Github，吾辈准确率不高，才57%左右 - [Github Link](https://github.com/HJTSO/image-classification/blob/master/dlnd_image_classification.ipynb "Title")   

#### 2.3. 卷积神经网络内部一窥
-------------------------------------
<center>![CNN](/image/DL/2/CNN.jpg)</center> 

-------------------------------------
瑞尔森大学的Adam Harley创建了一个交互式视觉化模型，能够帮助解释卷积神经网络内部每一层是如何工作的：[Link](http://scs.ryerson.ca/~aharley/vis/conv/ "Title")   

### 3. 自编码器（Autoencoder）

自编码器（Autoencoder）由于处理过程中有单元减少，解压缩效果不如MP3与JPEG，但是在图像去噪（Denoising）与降维（dimensionality reduction）方面取得不错的效果。

<center>![CNN](/image/DL/2/autoencoder_1.png)</center> 

-------------------------------------


-------------------------------------

### 参考资料（Reference）

- [零基础入门深度学习(7) - 递归神经网络](https://zybuluo.com/hanbingtao/note/626300 "Title") 

- [Reccusive Neural Networkを用いた文章と句の類似度算出](https://qiita.com/hiroto0227/items/ea1c723903a3e20a32e2 "Title") 

- [【Socher氏論文】推論にNeural Tensor（テンソル）Networkモデルの提案](https://qiita.com/HirofumiYashima/items/8ced35dcb437ed6aab6c "Title") 

- [Recursive Neural Network](http://www.iro.umontreal.ca/~bengioy/talks/gss2012-YB6-NLP-recursive.pdf"Title") 

- [Recursive Deep Models](https://nlp.stanford.edu/~socherr/EMNLP2013_RNTN.pdf "Title") 
