/*
 * Singly Linked List Implementation for Bank Queue
 * Requirement: FIFO (First In First Out)
 */

class Node {
    constructor(id, name) {
        this.customerId = id;
        this.customerName = name;
        this.next = null;
    }
}

class LinkedListQueue {
    constructor() {
        this.head = null;
        this.tail = null;
        this.length = 0;
        this.currentId = 1;
    }

    // Enqueue: Add to end
    enqueue(name) {
        const newNode = new Node(this.currentId++, name);
        if (!this.head) {
            this.head = newNode;
            this.tail = newNode;
        } else {
            this.tail.next = newNode;
            this.tail = newNode;
        }
        this.length++;
        return newNode;
    }

    // Dequeue: Remove from front
    dequeue() {
        if (!this.head) return null;

        const removedNode = this.head;
        this.head = this.head.next;

        // If queue is empty after dequeue, reset tail
        if (!this.head) {
            this.tail = null;
        }

        this.length--;
        return removedNode;
    }

    // Peek: View front
    peek() {
        return this.head;
    }

    // Convert to Array for display
    toArray() {
        const nodes = [];
        let current = this.head;
        while (current) {
            nodes.push(current);
            current = current.next;
        }
        return nodes;
    }

    isEmpty() {
        return this.length === 0;
    }
}

// UI Controller
const queue = new LinkedListQueue();

// DOM Elements
const nameInput = document.getElementById('customerName');
const queueDisplay = document.getElementById('queueDisplay');
const statusText = document.getElementById('statusText');
const currentServing = document.getElementById('currentServing');

function updateDisplay() {
    const nodes = queue.toArray();
    queueDisplay.innerHTML = '';

    if (nodes.length === 0) {
        queueDisplay.innerHTML = '<div class="empty-message">Queue is currently empty</div>';
        return;
    }

    nodes.forEach((node, index) => {
        const item = document.createElement('div');
        item.className = 'queue-item animate-in';
        // Add staggered animation delay
        item.style.animationDelay = `${index * 0.1}s`;

        item.innerHTML = `
            <div class="customer-avatar">
                ${node.customerName.charAt(0).toUpperCase()}
            </div>
            <div class="customer-info">
                <span class="id">#${node.customerId}</span>
                <span class="name">${node.customerName}</span>
            </div>
            ${index === 0 ? '<div class="badge">Next</div>' : ''}
        `;
        queueDisplay.appendChild(item);
    });
}

function handleEnqueue() {
    const name = nameInput.value.trim();
    if (!name) {
        statusText.textContent = "Please enter a customer name.";
        statusText.className = "status error";
        return;
    }

    const node = queue.enqueue(name);
    nameInput.value = '';
    statusText.textContent = `Added ${node.customerName} to queue.`;
    statusText.className = "status success";
    updateDisplay();
}

function handleDequeue() {
    if (queue.isEmpty()) {
        statusText.textContent = "Queue is empty! No one to serve.";
        statusText.className = "status error";
        return;
    }

    const node = queue.dequeue();
    currentServing.innerHTML = `
        <div class="serving-card animate-in">
            <h3>Now Serving</h3>
            <div class="large-id">#${node.customerId}</div>
            <div class="large-name">${node.customerName}</div>
        </div>
    `;
    statusText.textContent = `Serving ${node.customerName}...`;
    statusText.className = "status success";
    updateDisplay();
}

function handlePeek() {
    if (queue.isEmpty()) {
        statusText.textContent = "Queue is empty.";
        statusText.className = "status neutral";
        return;
    }

    const node = queue.peek();
    statusText.textContent = `Next in line: ${node.customerName} (#${node.customerId})`;
    statusText.className = "status neutral";
}

// Event Listeners
document.getElementById('btnEnqueue').addEventListener('click', handleEnqueue);
document.getElementById('btnDequeue').addEventListener('click', handleDequeue);
document.getElementById('btnPeek').addEventListener('click', handlePeek);
nameInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') handleEnqueue();
});

// Initial Render
updateDisplay();
