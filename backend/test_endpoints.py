import requests
import time

BASE_URL = "http://127.0.0.1:5500/api"

def test_flow():
    print("Testing Join Queue...")
    res = requests.post(f"{BASE_URL}/queue/join", json={"name": "Test User", "service_type": "Deposit"})
    print(res.json())
    assert res.status_code == 200
    assert "ticket_number" in res.json()
    ticket_num = res.json()['ticket_number']
    print(f"Joined with ticket {ticket_num}")

    print("\nTesting Status...")
    res = requests.get(f"{BASE_URL}/queue/status")
    print(res.json())
    assert res.json()['count'] > 0

    print("\nTesting Call Next (LIFO)...")
    res = requests.post(f"{BASE_URL}/teller/next")
    print(res.json())
    data = res.json()
    assert data['success'] == True
    assert data['ticket_number'] == ticket_num
    assert data['name'] == "Test User"

    print("\nTesting Transaction...")
    res = requests.post(f"{BASE_URL}/teller/transaction", json={"action": "Complete", "amount": 100})
    print(res.json())
    assert res.status_code == 200

if __name__ == "__main__":
    try:
        test_flow()
        print("\nALL TESTS PASSED")
    except Exception as e:
        print(f"\nTEST FAILED: {e}")
