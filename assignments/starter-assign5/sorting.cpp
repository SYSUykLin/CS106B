#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */

ListNode* divide(ListNode*& front, ListNode* end);
void concat(ListNode*& left, ListNode*& right);

void quickSort(ListNode*& front) {
    /* TODO: Implement this function. */
    if (!front || !(front->next))
    {
        return;
    }
    ListNode* end = front;
    while(end != nullptr)
    {
        end = end->next;
    }
    ListNode* mid = divide(front, end);

    ListNode* left = front;
    ListNode* right = mid->next;
    mid->next = nullptr;

    quickSort(left);
    quickSort(right);
    concat(left, right);

}

void swapData(ListNode*& p, ListNode*& q)
{
    int tmp = p->data;
    p->data = q->data;
    q->data = tmp;
}

ListNode* divide(ListNode*& front, ListNode* end)
{
    if (!front || front->next == nullptr)
    {
        return front;
    }
    int pivot = front->data;
    ListNode* left = front;
    ListNode* right = left->next;
    while(right != end)
    {
        if (right->data < pivot)
        {
            left = left->next;
            swapData(left, right);
        }
        right = right->next;
    }

    swapData(front, left);
    return left;
}

void concat(ListNode*& left, ListNode*& right)
{
    while(left->next != nullptr)
    {
        left = left->next;
    }
    left->next = right;
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    ListNode* pre = nullptr;
    while(front != nullptr)
    {
        pre = front;
        front = front->next;
        delete pre;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    ListNode* head = nullptr;
    ListNode* last = nullptr;
    for (int i = 0; i < values.size(); i++)
    {
        int v = values[i];
        if (i == 0)
        {
            head = new ListNode();
            head->data = v;
            head->next = nullptr;
            last = head;
        }
        else
        {
            ListNode* node = new ListNode();
            node->data = v;
            node->next = nullptr;
            last->next = node;
            last = node;
        }
    }
    return head;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){

    /**
     * @brief 两种情况，一种是vector比front长，第二种情况是front比vector长
     */

    ListNode* head = front;
    for(int e : v)
    {
        if (head == nullptr)
        {
            return false;
        }
        if (head->data != e)
        {
            return false;
        }

        head = head->next;
    }

    if (head != nullptr)
    {
        return false;
    }

    return true;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);
    /* Sort the list */
    quickSort(list);
    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test divide"){
    Vector<int> values = { 5, 4, 3, 1, 6, 7, 4, 9, 11, -9};
    ListNode* list = createList(values);
    ListNode* end = list;
    while(end->next != nullptr)
    {
        end = end->next;
    }
    divide(list, end);
    quickSort(list);
    printList(list);
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */
        EXPECT(areEquivalent(list, v));
        deallocateList(list);
    }
}

