/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
#include <math.h>
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */

    int iter_a = 0;
    int iter_b = 0;
    int length_a = a.size();
    int length_b = b.size();

    if (a.isEmpty())
    {
        return b;
    }
    if (b.isEmpty())
    {
        return a;
    }

    int a_pre = a.peek();
    int b_pre = b.peek();
    while(iter_a < length_a && iter_b < length_b)
    {
        if (a.peek() > b.peek())
        {
            result.enqueue(b.dequeue());
            ++iter_b;
            if (!b.isEmpty() && b.peek() < b_pre)
            {
                throw "b queue is not sorted";
            }
            else if (!b.isEmpty())
            {
                b_pre = b.peek();
            }        }
        else if (a.peek() < b.peek())
        {
            result.enqueue(a.dequeue());
            ++iter_a;
            if (!a.isEmpty() && a.peek() < a_pre)
            {
                throw "a queue is not sorted";
            }
            else if (!a.isEmpty())
            {
                a_pre = a.peek();
            }
        }
        else
        {
            result.enqueue(a.dequeue());
            result.enqueue(b.dequeue());
            ++iter_a;
            ++iter_b;

            if (!a.isEmpty() && a.peek() < a_pre)
            {
                throw "a queue is not sorted";
            }
            if (!b.isEmpty() && b.peek() < b_pre)
            {
                throw "b queue is not sorted";
            }
            if (!a.isEmpty())
            {
                a_pre = a.peek();
            }
            if (!b.isEmpty())
            {
                b_pre = b.peek();
            }
        }
    }
    while(iter_a < length_a)
    {
        result.enqueue(a.dequeue());
        ++iter_a;
        if (!a.isEmpty() && a.peek() < a_pre)
        {
            throw "a queue is not sorted";
        }
        if (!a.isEmpty())
        {
            a_pre = a.peek();
        }
    }
    while(iter_b < length_b)
    {
        result.enqueue(b.dequeue());
        ++iter_b;
        if (!b.isEmpty() && b.peek() < b_pre)
        {
            throw "b queue is not sorted";
        }
        if (!b.isEmpty())
        {
            b_pre = b.peek();
        }
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    /* TODO: Implement this function. */

    if (all.size() == 1)
    {
        return all[0];
    }

    Vector<Queue<int>> left = all.subList(0, all.size() / 2);
    Vector<Queue<int>> right = all.subList(all.size() / 2);
    Queue<int> left_merge = recMultiMerge(left);
    Queue<int> right_merge = recMultiMerge(right);
    result = binaryMerge(left_merge, right_merge);
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 2000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 22000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 44000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 180000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 360000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}



/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
