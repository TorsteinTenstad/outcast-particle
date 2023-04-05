#pragma once
#include <functional>
#include <iostream>
#include <vector>

struct UndoableAction
{
	std::function<void(void)> do_func;
	std::function<void(void)> undo_func;

	UndoableAction(std::function<void(void)>&& do_f, std::function<void(void)>&& undo_f) :
		do_func(std::move(do_f)),
		undo_func(std::move(undo_f))
	{}

	UndoableAction(const UndoableAction& other) = delete;
	UndoableAction(UndoableAction&& other) = delete;
	UndoableAction& operator=(const UndoableAction& other) = delete;
	UndoableAction& operator=(UndoableAction&& other) = delete;
};

class UndoSystem
{
private:
	std::list<UndoableAction> actions_;
	std::list<UndoableAction>::iterator next_action_;

public:
	UndoSystem()
	{
		next_action_ = actions_.end();
	}

	template <class... Args>
	void Do(Args... args)
	{
		actions_.erase(next_action_, actions_.end());
		actions_.emplace_back(std::forward<Args>(args)...);
		actions_.back().do_func();
		next_action_ = actions_.end();
	}
	bool Undo()
	{
		if (next_action_ == actions_.begin())
		{
			return false;
		}
		std::advance(next_action_, -1);
		next_action_->undo_func();
		return true;
	}

	bool Redo()
	{
		if (next_action_ == actions_.end())
		{
			return false;
		}
		next_action_->do_func();
		std::advance(next_action_, 1);
		return true;
	}

	void Clear()
	{
		actions_.clear();
		next_action_ = actions_.end();
	}
};