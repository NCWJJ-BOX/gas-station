# ระบบจำลองปั๊มน้ำมัน (Gas Station Simulator)

ระบบจำลองปั๊มน้ำมันพร้อมเว็บอินเตอร์เฟซสำหรับสั่งเติมน้ำมันและ Arduino สำหรับควบคุมอุปกรณ์จริง

## ภาพรวมระบบ

```
┌─────────────────┐         ┌─────────────────┐         ┌─────────────────┐
│   เว็บบราวเซอร์  │ ──────▶ │   FastAPI API   │ ◀────── │    Arduino      │
│  (สั่งเติมน้ำมัน) │         │   (Backend)    │         │  (ESP8266)      │
└─────────────────┘         └─────────────────┘         └─────────────────┘
```

- **Frontend**: เว็บอินเตอร์เฟซสำหรับเลือกประเภทน้ำมันและจำนวน
- **Backend**: FastAPI API สำหรับรับคำสั่งและจัดเก็บข้อมูล
- **Hardware**: Arduino (ESP8266) สำหรับดึงข้อมูลและควบคุมปั๊มน้ำมัน

## โครงสร้างโปรเจค

```
gas-station/
├── main.py                 # ไฟล์เริ่มต้น FastAPI
├── backend/
│   ├── __init__.py
│   ├── models.py          # Pydantic models
│   ├── data_store.py      # จัดการข้อมูล
│   └── routes.py          # API routes
├── QRCode/
│   ├── QRCode.ino        # Arduino main sketch
│   ├── config.h         # การตั้งค่า
│   ├── LCDManager.h     # จัดการ LCD
│   ├── DataFetcher.h    # ดึงข้อมูลจาก API
│   └── PumpController.h # ควบคุมปั๊ม
├── templates/
│   ├── form.html        # หน้าสั่งเติมน้ำมัน
│   └── qr_code.html     # หน้า QR Code
└── requirements.txt     # Python dependencies
```

## การติดตั้ง

### ข้อกำหนดเบื้องต้น

- Python 3.12 ขึ้นไป
- Arduino IDE (สำหรับ Arduino)

### 1. ติดตั้ง Python Dependencies

```bash
# Clone โปรเจค
git clone https://github.com/NCWJJ-BOX/gas-station.git
cd gas-station

# สร้าง virtual environment (แนะนำ)
python -m venv venv

# เปิดใช้งาน virtual environment
# Linux/Mac:
source venv/bin/activate
# Windows:
venv\Scripts\activate

# ติดตั้ง dependencies
pip install -r requirements.txt
```

### 2. รัน Backend Server

```bash
python main.py
```

Server จะทำงานที่ `http://localhost:8000`

### 3. เปิดใช้งานเว็บอินเตอร์เฟซ

เปิดบราวเซอร์และไปที่:
- **หน้าสั่งเติมน้ำมัน**: http://localhost:8000/
- **หน้า QR Code**: http://localhost:8000/qr-code

## การใช้งานเว็บอินเตอร์เฟซ

1. เลือกประเภทน้ำมัน (91, 95, หรือ เบนซิน)
2. กรอกจำนวนลิตรหรือจำนวนเงิน (ระบบจะคำนวณอัตโนมัติ)
3. ตรวจสอบยอดรวมใน Summary Card
4. กดปุ่ม "ยืนยันการสั่งซื้อ"
5. รอให้ Arduino ดึงข้อมูลไปเติมน้ำมัน

## การตั้งค่า Arduino

### Hardware Requirements

- ESP8266 (NodeMCU)
- LCD I2C 16x2 จำนวน 5 ตัว (Address: 0x27, 0x26, 0x25, 0x24, 0x23)
- Push Button 1 ตัว
- Relay Module 1 ตัว (สำหรับควบคุมปั๊ม)

### Libraries ที่ต้องติดตั้ง (Arduino IDE)

- ESP8266WiFi
- ESP8266HTTPClient
- NTPClient
- Wire
- LiquidCrystal_I2C
- ArduinoJson

### การตั้งค่า WiFi

แก้ไขไฟล์ `QRCode/config.h`:

```cpp
constexpr const char* WIFI_SSID     = "ชื่อWiFiของคุณ";
constexpr const char* WIFI_PASSWORD = "รหัสผ่านWiFi";
```

### การตั้งค่า Server URL

แก้ไข URL ของ server ใน `QRCode/config.h`:

```cpp
constexpr const char* SERVER_URL = "http://localhost:8000/data";
```

หากต้องการ deploy ให้เข้าถึงจากภายนอก ให้ใช้ public URL หรือ port forwarding

## API Endpoints

| Method | Path | คำอธิบาย |
|--------|------|----------|
| GET | `/` | หน้าหลัก (form) |
| POST | `/submit` | ส่งข้อมูลการสั่งซื้อน้ำมัน |
| GET | `/data` | ดึงข้อมูลการสั่งซื้อล่าสุด (สำหรับ Arduino) |
| GET | `/qr-code` | หน้าแสดง QR Code |

## ตัวอย่างการใช้งาน API

### ส่งข้อมูลการสั่งซื้อ

```bash
curl -X POST http://localhost:8000/submit \
  -H "Content-Type: application/json" \
  -d '{
    "fuel_name": "gas 95",
    "unit": "35.25",
    "liters": 10.0,
    "amount": 352.50
  }'
```

### ดึงข้อมูลการสั่งซื้อ

```bash
curl http://localhost:8000/data
```

Response:
```json
{
  "fuel_name": "gas 95",
  "unit": "35.25",
  "liters": 10.0,
  "amount": 352.5
}
```

## การ Deploy ขึ้น Production

### ใช้ PM2 (Linux)

```bash
# ติดตั้ง PM2
npm install -g pm2

# รันด้วย PM2
pm2 start main.py --name gas-station --interpreter python3

# ตั้งค่า auto-start
pm2 save
pm2 startup
```

### ใช้ Docker

สร้างไฟล์ `Dockerfile`:

```dockerfile
FROM python:3.12-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .
EXPOSE 8000

CMD ["python", "main.py"]
```

Build และ Run:

```bash
docker build -t gas-station .
docker run -d -p 8000:8000 --name gas-station gas-station
```

## Troubleshooting

### Backend ไม่ตอบสนอง

- ตรวจสอบว่า port 8000 ไม่ถูกใช้งานโดยโปรแกรมอื่น
- ตรวจสอบ firewall ว่าอนุญาต incoming connections

### Arduino ไม่ดึงข้อมูล

- ตรวจสอบการเชื่อมต่อ WiFi
- ตรวจสอบ URL ของ server ถูกต้องหรือไม่
- ดู Serial Monitor เพื่อดู error messages

### LCD ไม่แสดงผล

- ตรวจสอบ I2C address ของแต่ละ LCD
- ตรวจสอบการต่อสาย I2C (SDA, SCL)

## License

MIT License

## Contributing

ยินดีรับ Pull Requests! กรุณาสร้าง branch ใหม่สำหรับ feature ที่ต้องการเพิ่ม
