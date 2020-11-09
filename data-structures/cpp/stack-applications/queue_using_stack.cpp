#include<iostream>
#include<stack>

using namespace std;

template<class T>
class Queue {
    stack<T> *s1;
    stack<T> *s2;

    public:
    Queue() {
        s1 = new stack<int>();
        s2 = new stack<int>();
    }
    ~Queue() {
        delete s1;
        delete s2;
    }
    void enqueue(const T &data) {
        //push all elements from s1 into s2
        while(!s1->empty()) {
            s2->push(s1->top());
            s1->pop();
        }

        s1->push(data);

        //push all elements from s2 into s1
        while(!s2->empty()) {
            s1->push(s2->top());
            s2->pop();
        }
    }

    T front() {
        return s1->top();
    }

    void dequeue() {
        s1->pop();
    }

    bool isEmpty() {
        return s1->empty();
    }
};

int main() {
    Queue<int> q;

    q.enqueue(12);
    q.enqueue(14);
    q.enqueue(45);
    q.enqueue(456);
    
    while(!q.isEmpty()) {
        cout<<q.front()<<" ";
        q.dequeue();
    }
    cout<<endl;

    return 0;
}