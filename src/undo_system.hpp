#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

class UndoableAction
{
public:
	virtual void Do() = 0;
	virtual void Undo() = 0;

	// Derived actions may support the merging of actions. On a successful merge, the argument is consumed and an empty option is returned.
	// If the actions cannot be merged, the function does nothing and returns the argument.
	virtual std::optional<std::unique_ptr<UndoableAction>> TryMerge(std::unique_ptr<UndoableAction> other)
	{
		return other;
	}
};

class FunctionalUndoableAction : public UndoableAction
{
private:
	std::function<void(void)> do_func_;
	std::function<void(void)> undo_func_;

public:
	FunctionalUndoableAction(std::function<void(void)>&& do_func, std::function<void(void)>&& undo_func) :
		do_func_(std::move(do_func)),
		undo_func_(std::move(undo_func))
	{}

	void Do()
	{
		do_func_();
	}

	void Undo()
	{
		undo_func_();
	}

	FunctionalUndoableAction(const FunctionalUndoableAction& other) = delete;
	FunctionalUndoableAction(FunctionalUndoableAction&& other) = delete;
	FunctionalUndoableAction& operator=(const FunctionalUndoableAction& other) = delete;
	FunctionalUndoableAction& operator=(FunctionalUndoableAction&& other) = delete;
};

class UndoSystem
{
private:
	std::list<std::unique_ptr<UndoableAction>> actions_;
	std::list<std::unique_ptr<UndoableAction>>::iterator next_action_;

public:
	UndoSystem()
	{
		next_action_ = actions_.end();
	}

	void Do(std::unique_ptr<UndoableAction> action)
	{
		action->Do();
		actions_.erase(next_action_, actions_.end());

		std::optional<std::unique_ptr<UndoableAction>> unmerged_action = actions_.size() == 0 ?
			std::move(action) :
			actions_.back()->TryMerge(std::move(action));

		if (unmerged_action)
		{
			actions_.push_back(std::move(unmerged_action.value()));
		}
		next_action_ = actions_.end();
	}
	bool Undo()
	{
		if (next_action_ == actions_.begin())
		{
			return false;
		}
		std::advance(next_action_, -1);
		(*next_action_)->Undo();
		return true;
	}

	bool Redo()
	{
		if (next_action_ == actions_.end())
		{
			return false;
		}
		(*next_action_)->Do();
		std::advance(next_action_, 1);
		return true;
	}

	void Clear()
	{
		actions_.clear();
		next_action_ = actions_.end();
	}
};