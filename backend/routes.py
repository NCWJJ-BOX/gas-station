from fastapi import APIRouter, Body, HTTPException, Request
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates

from backend.models import FuelData, FuelResponse
from backend.data_store import data_store

router = APIRouter()
templates = Jinja2Templates(directory="templates")


@router.get("/", response_class=HTMLResponse)
async def read_form(request: Request, message: str | None = None):
    """Render the main form page."""
    return templates.TemplateResponse(
        "form.html", 
        {"request": request, "message": message}
    )


@router.post("/submit", response_model=FuelResponse)
async def submit_data(data: dict = Body(...)):
    """
    Receive fuel data from form submission.
    Validates and stores data for Arduino to fetch.
    """
    try:
        fuel_data = FuelData(
            fuel_name=data.get('fuel_name'),
            unit=data.get('unit'),
            liters=data.get('liters'),
            amount=data.get('amount')
        )
        
        data_store.save(fuel_data)
        
        print(f"Received data: {fuel_data.model_dump()}")
        
        return FuelResponse(
            message="Data received successfully",
            data=fuel_data
        )
        
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Internal server error: {str(e)}")


@router.get("/data")
async def get_data():
    """
    Endpoint for Arduino to fetch latest fuel data.
    Returns data and clears the store after delivery.
    """
    if data_store.is_empty():
        return {
            "fuel_name": "",
            "unit": "",
            "liters": 0,
            "amount": 0
        }
    
    return data_store.get()


@router.get("/qr-code", response_class=HTMLResponse)
async def generate_qr_code(request: Request):
    """Generate and display QR code for the web app URL."""
    import qrcode
    from io import BytesIO
    import base64
    
    url = "https://webpoo.ncwjjdx.work/"
    
    qr = qrcode.QRCode(version=1, box_size=10, border=5)
    qr.add_data(url)
    qr.make(fit=True)
    
    img = qr.make_image(fill='black', back_color='white')
    
    buf = BytesIO()
    img.save(buf)
    buf.seek(0)
    
    qr_code_b64 = base64.b64encode(buf.getvalue()).decode('utf-8')
    
    return templates.TemplateResponse(
        "qr_code.html", 
        {"request": request, "qr_code": qr_code_b64}
    )
