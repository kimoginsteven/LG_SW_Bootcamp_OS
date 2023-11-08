#include "irq_queue.h"
#include "device_driver.h"

struct IRQ_Node* front_uart_app0 = NULL;
struct IRQ_Node* rear_uart_app0 = NULL;
struct IRQ_Node* front_key3_app0 = NULL;
struct IRQ_Node* rear_key3_app0 = NULL;
struct IRQ_Node* front_key4_app0 = NULL;
struct IRQ_Node* rear_key4_app0 = NULL;
struct IRQ_Node* front_uart_app1 = NULL;
struct IRQ_Node* rear_uart_app1 = NULL;
struct IRQ_Node* front_key3_app1 = NULL;
struct IRQ_Node* rear_key3_app1 = NULL;
struct IRQ_Node* front_key4_app1 = NULL;
struct IRQ_Node* rear_key4_app1 = NULL;


void enqueue(struct IRQ_Node** front, struct IRQ_Node** rear, int data)
{
	struct IRQ_Node* new_node = (struct IRQ_Node*) malloc(sizeof(struct IRQ_Node));
	new_node->data = data;
	new_node->next = NULL;
    if (*front == NULL && *rear == NULL)
    {
    	*front = new_node;
    	*rear = new_node;

        return;
    }
    (*rear)->next = new_node;
    *rear = new_node;
}

int dequeue(struct IRQ_Node** front, struct IRQ_Node** rear)
{
    if (*front == NULL)
    {
        //Uart_Printf("Queue is empty\n");
        return 0;
    }
    struct IRQ_Node* temp = *front;
    int element = temp->data;
    if (*front == *rear)
    {
        *front = NULL;
        *rear = NULL;
    }
    else
    {
    	*front = (*front)->next;
    }
    free(temp);
    return element;
}

