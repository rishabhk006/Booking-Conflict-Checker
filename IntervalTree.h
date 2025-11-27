
#pragma once
#include <iostream>
#include <vector>
#include <json.hpp>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

struct Interval
{
    int start, end;
    Interval(int s, int e) : start(s), end(e) {}
};

struct IntervalNode
{
    Interval i;
    int max;
    IntervalNode *left;
    IntervalNode *right;

    IntervalNode(Interval interval) : i(interval), max(interval.end), left(nullptr), right(nullptr) {}

    ~IntervalNode() = default;
};

class IntervalTree
{
private:
    IntervalNode *root;

    bool doOverlap(Interval i1, Interval i2)
    {
        return (i1.start < i2.end && i2.start < i1.end);
    }

    IntervalNode *insert(IntervalNode *node, Interval i)
    {
        if (!node)
            return new IntervalNode(i);

        if (i.start < node->i.start)
            node->left = insert(node->left, i);
        else
            node->right = insert(node->right, i);

        node->max = max(node->max, i.end);
        return node;
    }

    bool overlapSearch(IntervalNode *node, Interval i)
    {
        if (!node)
            return false;
        if (doOverlap(node->i, i))
            return true;

        if (node->left && node->left->max >= i.start)
            return overlapSearch(node->left, i);

        return overlapSearch(node->right, i);
    }

    void collectIntervals(IntervalNode *node, json &intervals) const
    {
        if (!node)
            return;
        json interval;
        interval["start"] = node->i.start;
        interval["end"] = node->i.end;
        intervals.push_back(interval);
        collectIntervals(node->left, intervals);
        collectIntervals(node->right, intervals);
    }

    IntervalNode *remove(IntervalNode *node, Interval i)
    {
        if (!node)
            return nullptr;

        if (i.start < node->i.start)
        {
            node->left = remove(node->left, i);
        }
        else if (i.start > node->i.start || i.end != node->i.end)
        {
            node->right = remove(node->right, i);
        }
        else
        {
            // Node found
            if (!node->left)
            {
                IntervalNode *temp = node->right;
                node->right = nullptr; // prevent recursive delete
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                IntervalNode *temp = node->left;
                node->left = nullptr;
                delete node;
                return temp;
            }

            // Node with two children: find inorder successor
            IntervalNode *succ = node->right;
            while (succ->left)
                succ = succ->left;
            node->i = succ->i;
            node->right = remove(node->right, succ->i);
        }

        // Update max
        node->max = node->i.end;
        if (node->left)
            node->max = std::max(node->max, node->left->max);
        if (node->right)
            node->max = std::max(node->max, node->right->max);

        return node;
    }

public:
    IntervalTree() : root(nullptr) {}
    ~IntervalTree() = default;

    void insert(int start, int end)
    {
        root = insert(root, Interval(start, end));
    }

    bool overlaps(int start, int end)
    {
        return overlapSearch(root, Interval(start, end));
    }

    bool hasBookings() const
    {
        return root != nullptr;
    }

    void remove(int start, int end)
    {
        root = remove(root, Interval(start, end));
    }

    json getIntervals() const
    {
        json intervals = json::array();
        collectIntervals(root, intervals);
        return intervals;
    }
};