#pragma once

#include <iostream>
#include <string>
#include "../DSA/Stack.h"
using namespace std;

/*
Feature 3: Stack — Code Undo System
Provides interactive Undo/Redo code editing operations backed by two custom Stacks.
*/

class UndoRedoManager
{
    private:
        Stack<string> undoStack;
        Stack<string> redoStack;
        string currentCode;

    public:
        UndoRedoManager(const string& initialCode = "")
        {
            currentCode = initialCode;
        }

        // Edit or type code: records current snapshot to undoStack, clears redoStack
        void typeCode(const string& newCode)
        {
            undoStack.push(currentCode);
            redoStack.clear();
            currentCode = newCode;
        }

        // Undo: restores previous code snapshot
        bool undo()
        {
            if (undoStack.isEmpty())
            {
                cout << "[UndoRedo] Nothing to undo!\n";
                return false;
            }

            redoStack.push(currentCode);
            currentCode = undoStack.pop();
            return true;
        }

        // Redo: re-applies undone code snapshot
        bool redo()
        {
            if (redoStack.isEmpty())
            {
                cout << "[UndoRedo] Nothing to redo!\n";
                return false;
            }

            undoStack.push(currentCode);
            currentCode = redoStack.pop();
            return true;
        }

        string getCurrentCode() const
        {
            return currentCode;
        }

        bool canUndo() const
        {
            return !undoStack.isEmpty();
        }

        bool canRedo() const
        {
            return !redoStack.isEmpty();
        }

        int getUndoHistorySize() const
        {
            return undoStack.getSize();
        }

        int getRedoHistorySize() const
        {
            return redoStack.getSize();
        }

        void reset(const string& initialCode = "")
        {
            undoStack.clear();
            redoStack.clear();
            currentCode = initialCode;
        }

        void displayEditorState() const
        {
            cout << "\n=========================== CODE EDITOR STATE ===========================\n";
            cout << " Undo Stack Depth: " << undoStack.getSize()
                 << " | Redo Stack Depth: " << redoStack.getSize()
                 << " | Can Undo: " << (canUndo() ? "Yes" : "No")
                 << " | Can Redo: " << (canRedo() ? "Yes" : "No") << "\n";
            cout << "-------------------------------------------------------------------------\n";
            cout << " Current Code Content:\n";
            cout << currentCode << "\n";
            cout << "=========================================================================\n\n";
        }
};
