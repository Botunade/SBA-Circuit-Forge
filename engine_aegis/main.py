from fastapi import FastAPI

app = FastAPI(title="SBA Aegis (AI Mentor Backend)")

@app.get("/")
def read_root():
    return {"message": "SBA Aegis Mentor Backend is running. Awaiting protobuf messages..."}
