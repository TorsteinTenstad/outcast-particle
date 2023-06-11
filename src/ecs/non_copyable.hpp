#pragma once

// Not copied between two EntityContainers
class NonCopyableExternally
{};

// Not copied to a new Entity in the same EntityContainer
class NonCopyableInternally
{};

// Not copied to a new Entity in the same EntityContainer or between two EntityContainers
class NonCopyable : public NonCopyableInternally, public NonCopyableExternally
{};