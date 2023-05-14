#pragma once
#include <memory>
#include <stack>

template <class T>
class MultiStateObject;

template <class T>
class MultiState
{
private:
	std::stack<std::unique_ptr<T>> states_;

private:
	T& Push()
	{
 		states_.push(std::make_unique<T>());
		states_.top()->owner_ = this;
		return *states_.top();
	}

	void Pop()
	{
		assert(states_.size() > 1);
		states_.pop();
	}

	bool TryPop()
	{
		if (!(states_.size() > 1)) { return false; }
		Pop();
		return true;
	}

public:
	MultiState()
	{
		Push();
	}

	T& GetActiveState()
	{
		return *states_.top();
	}
	void Reset()
	{
		while (!states_.empty())
			states_.pop();
		Push();
	}

	friend class MultiStateObject<T>;
};

template <class T>
class MultiStateObject
{
private:
	MultiState<T>* owner_;

public:
	T& Push() { return owner_->Push(); }
	void Pop() { return owner_->Pop(); }
	bool TryPop() { return owner_->TryPop(); }

	friend class MultiState<T>;
};