# test_env.py
import tensorflow as tf
import numpy as np

print("TensorFlow版本:", tf.__version__)
print("是否支持GPU:", tf.test.is_gpu_available() if hasattr(tf.test, 'is_gpu_available') else 'Check with tf.config.list_physical_devices("GPU")')
print("可用设备:", tf.config.list_physical_devices())

# 简单的TensorFlow操作测试
a = tf.constant([1, 2, 3])
b = tf.constant([4, 5, 6])
c = tf.add(a, b)
print("TensorFlow计算测试:", c.numpy())
