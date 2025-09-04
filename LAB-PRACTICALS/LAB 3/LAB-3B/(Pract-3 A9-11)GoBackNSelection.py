import random
import time

class Sender:
    def __init__(self, window_size=4, timeout=2):
        self.window_size = window_size
        self.timeout = timeout
        self.base = 0
        self.next_seq_num = 0
        self.acks_received = []
        self.packets = []
        self.timers = {}

    def set_packets(self, packets):
        self.packets = packets
        self.acks_received = [False] * len(packets)

    def send_packet(self, seq_num):
        if seq_num >= len(self.packets):
            return None

        if random.random() < 0.1:
            print(f"Packet {seq_num} lost!")
            return None

        print(f"Send Packet {seq_num}: '{self.packets[seq_num]}'")
        return (seq_num, self.packets[seq_num])

    def receive_ack(self, ack_num):
        if random.random() < 0.1:
            print(f"ACK {ack_num} lost!")
            return False

        print(f"Received ACK {ack_num}")
        if ack_num < len(self.acks_received):
            self.acks_received[ack_num] = True

        while self.base < len(self.acks_received) and self.acks_received[self.base]:
            self.base += 1

        return True

    def run(self, receiver):
        print("Starting transmission...")
        print("="*40)

        while self.base < len(self.packets):
            while self.next_seq_num < self.base + self.window_size and self.next_seq_num < len(self.packets):
                packet = self.send_packet(self.next_seq_num)
                if packet:
                    ack = receiver.receive_packet(packet)
                    if ack is not None:
                        self.receive_ack(ack)
                self.next_seq_num += 1

            current_time = time.time()
            timeout_occurred = False
            for seq_num in range(self.base, min(self.next_seq_num, len(self.packets))):
                if not self.acks_received[seq_num]:
                    if seq_num not in self.timers:
                        self.timers[seq_num] = current_time
                    elif current_time - self.timers[seq_num] > self.timeout:
                        print(f"Timeout for packet {seq_num}, resending from {seq_num}")
                        self.next_seq_num = seq_num
                        timeout_occurred = True
                        break
            if not timeout_occurred:
                time.sleep(0.5)

        print("="*40)
        print("Transmission Complete!")

class Receiver:
    def __init__(self):
        self.expected_seq_num = 0
        self.received_packets = {}

    def receive_packet(self, packet):
        if packet is None:
            return None

        seq_num, data = packet
        print(f"Received Packet {seq_num}: '{data}'")

        if seq_num == self.expected_seq_num:
            self.received_packets[seq_num] = data
            self.expected_seq_num += 1
            print(f"Sending ACK {seq_num}")
            return seq_num
        else:
            print(f"Out-of-order packet {seq_num}, expecting {self.expected_seq_num}")
            print(f"Resending ACK {self.expected_seq_num - 1}")
            return self.expected_seq_num - 1

def main():
    data = "Hello World"
    packets = list(data)

    print(f"Data to be transmitted : {data}")

    sender = Sender(window_size=4, timeout=2)
    receiver = Receiver()

    sender.set_packets(packets)

    sender.run(receiver)

    received_data = []
    for i in range(len(packets)):
        if i in receiver.received_packets:
            received_data.append(receiver.received_packets[i])

    print(f"Final received data: {''.join(received_data)}")

if __name__ == "__main__":
    main()
