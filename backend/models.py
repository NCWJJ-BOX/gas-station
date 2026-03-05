from pydantic import BaseModel, Field, field_validator


class FuelData(BaseModel):
    """Model for fuel dispensing data."""
    fuel_name: str = Field(..., min_length=1, max_length=50)
    unit: str = Field(..., min_length=1, max_length=20)
    liters: float = Field(..., ge=0)
    amount: float = Field(..., ge=0)

    @field_validator('liters', 'amount', mode='before')
    @classmethod
    def convert_to_float(cls, v):
        if isinstance(v, str):
            return float(v)
        return v

    @field_validator('fuel_name', 'unit', mode='before')
    @classmethod
    def convert_to_str(cls, v):
        if v is None:
            return ""
        return str(v)


class FuelResponse(BaseModel):
    """Response model for successful operations."""
    message: str
    data: FuelData | None = None


class ErrorResponse(BaseModel):
    """Response model for errors."""
    detail: str
