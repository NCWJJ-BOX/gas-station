import uvicorn
from fastapi import FastAPI
from backend.routes import router as api_router


def create_app() -> FastAPI:
    """Create and configure FastAPI application."""
    app = FastAPI(
        title="Gas Station API",
        description="API for gas station simulator",
        version="1.0.0"
    )
    
    app.include_router(api_router)
    
    return app


app = create_app()


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
