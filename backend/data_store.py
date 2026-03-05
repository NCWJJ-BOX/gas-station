"""
Data store service for gas station.
Currently uses in-memory storage. Can be swapped to database later.
"""
from typing import Optional
from backend.models import FuelData


class DataStore:
    """In-memory data store for fuel transactions."""
    
    def __init__(self):
        self._store: dict = {}
    
    def save(self, data: FuelData) -> None:
        """Save fuel data to store."""
        self._store = data.model_dump()
    
    def get(self) -> dict:
        """Get all data and clear store."""
        data = self._store.copy()
        self._store.clear()
        return data
    
    def is_empty(self) -> bool:
        """Check if store is empty."""
        return len(self._store) == 0
    
    def get_latest(self) -> Optional[dict]:
        """Get latest data without clearing."""
        if self._store:
            return self._store.copy()
        return None


# Singleton instance
data_store = DataStore()
