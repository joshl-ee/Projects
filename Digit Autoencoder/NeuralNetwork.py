import numpy as np
import mnist
from tensorflow import keras
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.utils import to_categorical

# Images for training
train_images = mnist.train_images()
train_labels = mnist.train_labels()

# Images for testing
test_images = mnist.test_images()
test_labels = mnist.test_labels()

# Normalize pixels [0, 255] -> [-0.5, 0.5]
train_images = (train_images / 225) - 0.5
test_images = (test_images / 225) - 0.5

# Flatten images
train_images = train_images.reshape((-1, 784))
test_images = test_images.reshape((-1, 784))

# Create model of 3 layers
model = Sequential([
    Dense(64, activation='relu', input_shape=(784,)),      #1st layer: 64 nodes
    Dense(64, activation='relu'),      #2nd layer: 64 nodes
    Dense(10, activation='softmax'),   #3rd layer: 10 Softmax layer (probability distribution)
])

# Compilation of Model
model.compile(
    optimizer='adam',
    loss='categorical_crossentropy',
    metrics=['accuracy'],
)

# Training the model
model.fit(
    train_images,
    to_categorical(train_labels),
    epochs=5,
    batch_size=32,
)

# Evaluate the model
model.evaluate(
    test_images,
    to_categorical(test_labels)
)

model.save_weights('model.h5')

# Save model to disk
#model.load_weights('model.h5')

# Predict first 5 test images
prediction = model.predict(test_images[:5])

# Print out model predictions
print(np.argmax(prediction, axis=1))

#Print actual values
print(test_labels[:5])
