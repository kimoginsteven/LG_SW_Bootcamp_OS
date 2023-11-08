struct IRQ_Node {
    int data;
    struct IRQ_Node* next;
};

extern struct IRQ_Node* front_uart_app0;
extern struct IRQ_Node* rear_uart_app0;
extern struct IRQ_Node* front_key3_app0;
extern struct IRQ_Node* rear_key3_app0;
extern struct IRQ_Node* front_key4_app0;
extern struct IRQ_Node* rear_key4_app0;
extern struct IRQ_Node* front_uart_app1;
extern struct IRQ_Node* rear_uart_app1;
extern struct IRQ_Node* front_key3_app1;
extern struct IRQ_Node* rear_key3_app1;
extern struct IRQ_Node* front_key4_app1;
extern struct IRQ_Node* rear_key4_app1;

extern void enqueue(struct IRQ_Node** front, struct IRQ_Node** rear, int data);
extern int dequeue(struct IRQ_Node** front, struct IRQ_Node** rear);
