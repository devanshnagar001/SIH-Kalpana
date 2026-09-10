"""
train.py
Trains the lightweight keyword-spotting (KWS) model for the "Kalpana"
wake word and exports a quantized TFLite Micro model.

Input tensor shape and dtype must match contract/contract_v1.md:
  [49, 13, 1] int8

Submission-stage scaffold: model architecture and training loop are
defined and runnable against a placeholder dataset path; swap
DATA_DIR for the real recorded dataset (see data/README.md) before
training for accuracy.
"""

import os
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models

DATA_DIR = "../../data"
MODEL_OUT_DIR = "../exported"
INPUT_SHAPE = (49, 13, 1)   # time steps x MFCC coeffs x channel
NUM_CLASSES = 2              # 0 = background/other, 1 = "kalpana"
BATCH_SIZE = 32
EPOCHS = 20


def build_model(input_shape=INPUT_SHAPE, num_classes=NUM_CLASSES) -> tf.keras.Model:
    """Small depthwise-separable CNN — DS-CNN style, sized for microcontroller inference."""
    inputs = layers.Input(shape=input_shape)
    x = layers.Conv2D(16, (3, 3), padding="same", activation="relu")(inputs)
    x = layers.BatchNormalization()(x)
    x = layers.MaxPooling2D((2, 2))(x)

    x = layers.SeparableConv2D(32, (3, 3), padding="same", activation="relu")(x)
    x = layers.BatchNormalization()(x)
    x = layers.MaxPooling2D((2, 2))(x)

    x = layers.SeparableConv2D(32, (3, 3), padding="same", activation="relu")(x)
    x = layers.BatchNormalization()(x)
    x = layers.GlobalAveragePooling2D()(x)

    x = layers.Dense(32, activation="relu")(x)
    x = layers.Dropout(0.3)(x)
    outputs = layers.Dense(num_classes, activation="softmax")(x)

    return models.Model(inputs, outputs, name="kalpana_kws")


def load_dataset(data_dir=DATA_DIR):
    """
    Expects precomputed MFCC feature arrays under:
      data_dir/kalpana/*.npy   (positive examples)
      data_dir/background/*.npy (negative examples)
    Each .npy holds a [49, 13] float array. See data/README.md.

    Placeholder: returns a small synthetic dataset so the training script
    runs end-to-end without the real dataset present.
    """
    if not os.path.isdir(data_dir) or not os.listdir(data_dir):
        print(f"[train.py] No dataset found at {data_dir}, using synthetic placeholder data.")
        n = 200
        x = np.random.randn(n, *INPUT_SHAPE).astype(np.float32)
        y = np.random.randint(0, NUM_CLASSES, size=n)
        return x, y

    # TODO: real loader — walk data_dir/kalpana and data_dir/background,
    # stack into (x, y) arrays.
    raise NotImplementedError("Real dataset loader not yet implemented.")


def representative_dataset_gen(x_train):
    for i in range(min(100, len(x_train))):
        yield [x_train[i:i + 1].astype(np.float32)]


def main():
    x, y = load_dataset()
    model = build_model()
    model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
    model.summary()

    model.fit(x, y, batch_size=BATCH_SIZE, epochs=EPOCHS, validation_split=0.2)

    os.makedirs(MODEL_OUT_DIR, exist_ok=True)

    # Post-training int8 quantization for TFLite Micro
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = lambda: representative_dataset_gen(x)
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.int8
    converter.inference_output_type = tf.int8

    tflite_model = converter.convert()

    out_path = os.path.join(MODEL_OUT_DIR, "kalpana_kws.tflite")
    with open(out_path, "wb") as f:
        f.write(tflite_model)

    print(f"[train.py] Exported quantized model to {out_path}")


if __name__ == "__main__":
    main()
