# ds_bank

This repository is a skeleton for a bank queue simulation project.

Directory structure

- `src/` - C source files. Implement your simulation in `src/bankqueue.c`.
- `build/` - Compiled executables (e.g., `bankqueue.exe` on Windows).
- `frontend/` - Optional frontend mockup (`index.html`, `style.css`).
- `backend/` - Flask backend with C library integration
- `pure_js_bank/` - Pure JavaScript implementation using singly linked list

## Features

### Queue Operations
- **Enqueue**: Add customers to the queue with name and service type
- **Dequeue**: Serve the next customer (FIFO - First In, First Out)
- **Peek**: View who is next in line without removing them
- **Revoke/Cancel**: Remove a specific ticket from the queue by ticket number

### How to Revoke a Ticket

Customers can cancel their queue position using the ticket number they received when joining:

1. **Web Interface** (Flask + Frontend):
   - On the Customer Kiosk view, scroll to the "Cancel Your Ticket" section
   - Enter your ticket number
   - Click "Cancel Ticket"
   - The system will remove your position from the queue

2. **API Endpoint**:
   ```bash
   POST /api/queue/revoke
   Content-Type: application/json
   
   {
     "ticket_number": 5
   }
   ```

3. **Pure JS Implementation**:
   - Enter the Customer ID in the "Cancel/Revoke a Ticket" field
   - Click "Revoke" button

Getting started (example using GCC on Windows / MinGW):

1. Install a C compiler (MinGW-w64 or Visual Studio build tools).
2. From PowerShell, run:

```powershell
gcc -o build\bankqueue.exe src\bankqueue.c
```

3. Run the program:

```powershell
build\bankqueue.exe
```

Notes

- This is only a project skeleton. Implement the queue simulation and tests in `src/`.
- Place compiled binaries into `build/`.

Happy coding!
