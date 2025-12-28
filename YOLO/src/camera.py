from ultralytics import YOLO
import cv2

# ===================== 1. 加载唯一的YOLO模型（仅task.pt） =====================
# 替换为你实际的task.pt权重路径
model = YOLO(r"C:\Users\hyper\Desktop\program\.Gsing\YOLO\weights\task2.pt")

# ===================== 2. 摄像头初始化 =====================
cap = cv2.VideoCapture(1)  # 摄像头索引：0=默认摄像头，1=外接摄像头（根据你的设备调整）
#cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)  # 摄像头画面宽度
#cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)  # 摄像头画面高度

if not cap.isOpened():
    print("错误：无法打开摄像头！")
    exit()
print("摄像头已启动，按 'q' 键退出识别...")

# ===================== 3. 单模型实时推理循环 =====================
while True:
    ret, frame = cap.read()
    if not ret:
        print("错误：无法读取摄像头画面！")
        break

    # -------------------- 执行task模型推理 --------------------
    results = model(
        source=frame,
        imgsz=960,
        device=0,
        conf=0.5,  # ① 提高置信度阈值（过滤低置信度的不稳定框）
        iou=0.5,   # ② 适度提高IOU阈值（减少重复框）
        verbose=False,
        max_det=50, # ③ 限制最大检测数量（避免过多冗余框）
        agnostic_nms=True # ④ 类别无关NMS（减少同类重复框，对多类别检测友好）
    )

    # -------------------- 绘制检测结果到画面 --------------------
    annotated_frame = results[0].plot()  # 直接绘制task模型的检测框+类别

    # -------------------- 显示与退出控制 --------------------
    cv2.imshow("YOLO task模型 摄像头实时识别", annotated_frame)
    # 按q键退出（等待1ms，保证画面流畅）
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# ===================== 4. 释放资源 =====================
cap.release()  # 释放摄像头
cv2.destroyAllWindows()  # 关闭所有窗口
print("摄像头已关闭，识别结束！")