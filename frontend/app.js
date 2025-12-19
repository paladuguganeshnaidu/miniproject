const API_BASE = '/api';

// --- View Switching ---
function switchView(viewName) {
    document.querySelectorAll('.view-section').forEach(el => el.classList.remove('active'));
    document.querySelectorAll('.switch-btn').forEach(el => el.classList.remove('active'));

    document.getElementById(`${viewName}View`).classList.add('active');
    // Find the button with onclick containing the viewName
    const buttons = document.querySelectorAll('.switch-btn');
    if (viewName === 'kiosk') buttons[0].classList.add('active');
    else buttons[1].classList.add('active');

    if (viewName === 'teller') updateStats();
}

// --- Status Updates ---
async function updateStats() {
    try {
        const res = await fetch(`${API_BASE}/queue/status`);
        const data = await res.json();
        document.getElementById('queueCount').innerText = data.count;
    } catch (e) {
        console.error("Stats error", e);
    }
}

// Polling for stats when not in kiosk mode
setInterval(() => {
    if (!document.getElementById('kioskView').classList.contains('active')) {
        updateStats();
    }
}, 3000);


// --- Kiosk Logic ---
async function handleJoinQueue(e) {
    e.preventDefault();
    const name = document.getElementById('custName').value;
    const type = document.querySelector('input[name="service"]:checked').value;

    if (!name) return showToast('Please enter your name');

    try {
        const res = await fetch(`${API_BASE}/queue/join`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name: name, service_type: type })
        });

        if (res.ok) {
            const data = await res.json();
            // Show Animation
            document.getElementById('ticketName').innerText = name;
            document.getElementById('ticketService').innerText = type;
            document.getElementById('ticketNum').innerText = `A-${data.ticket_number}`;

            // Re-trigger animation
            const ticket = document.getElementById('printedTicket');
            ticket.style.animation = 'none';
            ticket.offsetHeight; /* trigger reflow */
            ticket.style.animation = '';

            document.getElementById('ticketOverlay').classList.remove('hidden');

            // Reset Form
            document.getElementById('custName').value = '';
        }
    } catch (e) {
        showToast('Connection Failed');
    }
}

function closeTicket() {
    document.getElementById('ticketOverlay').classList.add('hidden');
    showToast('You are in the queue!');
}

// --- Teller Logic ---
async function callNextCustomer() {
    try {
        const res = await fetch(`${API_BASE}/teller/next`, { method: 'POST' });
        const data = await res.json();

        if (data.success) {
            document.getElementById('emptyState').classList.add('hidden');
            document.getElementById('activeState').classList.remove('hidden');

            document.getElementById('serveName').innerText = data.name;
            document.getElementById('serveType').innerText = data.service_type;
            document.getElementById('serveTicket').innerText = `#${data.ticket_number}`;
        } else {
            showToast('Queue is empty!');
            document.getElementById('activeState').classList.add('hidden');
            document.getElementById('emptyState').classList.remove('hidden');
        }
        updateStats();
    } catch (e) {
        showToast('Error calling next');
    }
}

async function processTxn(action) {
    const amount = document.getElementById('txnAmount').value;

    try {
        const res = await fetch(`${API_BASE}/teller/transaction`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ action, amount })
        });
        const data = await res.json();

        showToast(data.message);
    } catch {
        showToast('Transaction Failed');
    }

    // Reset UI
    document.getElementById('txnAmount').value = '';
    document.getElementById('activeState').classList.add('hidden');
    document.getElementById('emptyState').classList.remove('hidden');
    updateStats();
}

function showToast(msg) {
    const t = document.getElementById('toast');
    t.innerText = msg;
    t.classList.remove('hidden');
    setTimeout(() => t.classList.add('hidden'), 3000);
}

// Initial stats load
updateStats();
