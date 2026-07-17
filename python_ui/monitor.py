import time
import subprocess
import sys
import os
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

PYTHON_INTERPRETER = sys.executable
APP_SCRIPT = "main.py"
WATCH_DIRECTORY = os.path.dirname(os.path.abspath(__file__)) # This script's directory

current_process = None

def start_app():
    global current_process
    if current_process:
        print("Terminating existing app process...")
        current_process.terminate()
        current_process.wait()

    print(f"Starting {APP_SCRIPT}...")
    current_process = subprocess.Popen([PYTHON_INTERPRETER, APP_SCRIPT], cwd=WATCH_DIRECTORY)
    print(f"Process ID: {current_process.pid}")

class MyEventHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if not event.is_directory and event.src_path.endswith(".py"):
            print(f"Detected change in {event.src_path}. Restarting app...")
            start_app()

if __name__ == "__main__":
    # Initial start
    start_app()

    event_handler = MyEventHandler()
    observer = Observer()
    observer.schedule(event_handler, WATCH_DIRECTORY, recursive=True)
    observer.start()

    print(f"Monitoring directory: {WATCH_DIRECTORY}")
    print("Press Ctrl+C to stop monitoring.")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
        if current_process:
            print("Terminating final app process...")
            current_process.terminate()
            current_process.wait()
    observer.join()
    print("Monitor stopped.")
