
import sqlite3
import os
from flask import Flask, request, jsonify, send_from_directory
import os
import ctypes
import sys

app = Flask(__name__, static_folder='../frontend', static_url_path='')

# Load C Library
lib_name = 'banckqueue.dll' if os.name == 'nt' else 'libbanckqueue.so'
dll_path = os.path.join(os.path.dirname(__file__), lib_name)

# Auto-compile for Linux if missing (Fallback if build script didn't run)
if os.name != 'nt' and not os.path.exists(dll_path):
    print("Library not found. Attempting to compile...")
    src_path = os.path.join(os.path.dirname(__file__), 'banckqueue.c')
    os.system(f"gcc -shared -o {dll_path} {src_path} -fPIC")

try:
    c_queue = ctypes.CDLL(dll_path)
except OSError as e:
    print(f"Error loading Library from {dll_path}: {e}")
    # Fallback to try finding it in current directory if absolute path fails
    try:
        c_queue = ctypes.CDLL(lib_name)
    except OSError:
        print("CRITICAL: Could not load C library. Exiting.")
        sys.exit(1)

# Define C structures and return types
class CustomerData(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char * 100),
        ("service_type", ctypes.c_char * 100),
        ("ticket_number", ctypes.c_int),
        ("success", ctypes.c_int)
    ]

c_queue.enqueue.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
c_queue.enqueue.restype = ctypes.c_int

c_queue.dequeue.argtypes = []
c_queue.dequeue.restype = CustomerData

c_queue.peek.argtypes = []
c_queue.peek.restype = CustomerData

c_queue.get_count.argtypes = []
c_queue.get_count.restype = ctypes.c_int

@app.route('/')
def serve_index():
    return send_from_directory('../frontend', 'index.html')

@app.route('/<path:path>')
def serve_static(path):
    return send_from_directory('../frontend', path)

# --- API Endpoints ---

@app.route('/api/queue/join', methods=['POST'])
def join_queue():
    data = request.json
    name = data.get('name', 'Anonymous').encode('utf-8')
    service_type = data.get('service_type', 'General').encode('utf-8')
    
    ticket_num = c_queue.enqueue(name, service_type)
    
    return jsonify({'message': 'Joined queue', 'status': 'waiting', 'ticket_number': ticket_num})

@app.route('/api/queue/status', methods=['GET'])
def get_queue_status():
    count = c_queue.get_count()
    return jsonify({'count': count})

@app.route('/api/teller/next', methods=['POST'])
def call_next():
    # LIFO Pop
    data = c_queue.dequeue()
    
    if data.success:
        return jsonify({
            'success': True,
            'name': data.name.decode('utf-8'),
            'service_type': data.service_type.decode('utf-8'),
            'ticket_number': data.ticket_number
        })
    else:
        return jsonify({'success': False, 'message': 'Queue is empty'})

@app.route('/api/teller/transaction', methods=['POST'])
def perform_transaction():
    # Since we have no DB, we just mock the success of the operation
    data = request.json
    action = data.get('action')
    amount = data.get('amount')
    
    # In a real app, this would update a database
    return jsonify({
        'message': f'Successfully processed {action} of ${amount}',
        'new_balance': 1234.56 # Mock balance
    })

if __name__ == '__main__':
    app.run(debug=True, port=5500)

