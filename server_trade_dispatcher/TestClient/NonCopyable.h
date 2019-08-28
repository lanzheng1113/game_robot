#pragma once

class NonCopyable {
protected:

	NonCopyable(){;}

	~NonCopyable(){;}

private:

	/**
	 * Copy constructor is private.
	 */
	NonCopyable(const NonCopyable &);

	/**
	 * Copy assignement is private.
	 */
	const NonCopyable & operator=(const NonCopyable &);
};