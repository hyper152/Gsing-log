from ultralytics import YOLO  # 导入Ultralytics的YOLOv8核心类
import os                     # 导入os库，用于路径处理和文件存在性判断

# ===================== 全局配置：路径与权重设置 =====================
# 1. 你的data.yaml
DATA = r"C:/Users/hyper/Desktop/program/.Gsing/YOLO/data.yaml"
    
# 2. 权重路径（简化，避免层级错误）
WEIGHTS_DIR = r"C:\Users\hyper\Desktop\program\.Gsing\YOLO\weights"
os.makedirs(WEIGHTS_DIR, exist_ok=True)
WEIGHTS = os.path.join(WEIGHTS_DIR, 'yolov8s.pt')

# ===================== 主训练逻辑 =====================
if __name__ == '__main__':
    # 加载权重（本地优先，否则自动下载）
    model_path = WEIGHTS if os.path.exists(WEIGHTS) else 'yolov8s.pt'
    print(f'加载权重：{model_path}')
    
    # 加载YOLOv8模型
    model = YOLO(model_path)
    
    
    # 启动训练（参数不变，核心是data.yaml已正确）
    model.train(
        data=DATA,               # 指向正确的data.yaml
        epochs=150,              # 4类别小数据集建议100轮
        imgsz=640,               # 适配1080P图片
        batch=4,                # Windows稳定值Y
        device=0,                # GPU训练，无GPU改'cpu'
        workers=0,               # Windows必须设0
        pretrained=True,         # 小数据集必备
        augment=True,
        amp=False            # 数据增强 
    )
