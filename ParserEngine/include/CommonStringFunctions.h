/*
 * CommonStringFunctions.h - Some helpful string functions.
 *
 *  Created on:     10.11.2014
 *  Last modified:  23.10.2016
 *
 *      Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>

	Copyright 2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 */

#ifndef COMMONSTRINGFUNCTIONS_H_
#define COMMONSTRINGFUNCTIONS_H_

// Some helpful string functions.
    // Find 'findText' in 'text' and then replace it with 'replaceText'.
    // Return 'text'. Also change 'text'.
	std::string FindNReplace(std::string &asText, std::string asFindText, std::string asReplaceText)
	{
		if (asFindText == "")	// Return when got null string.
			return asText;

		std::string::size_type Pos = asText.find(asFindText);		// Get start position for replace text.
		std::string::size_type ReplaceSize = asFindText.size();	// Get size for replace text.

		if (!ReplaceSize)	// For null ReplaceSize set it to 1.
			ReplaceSize = 1;

		while (Pos < asText.size() && Pos != std::string::npos) {
			asText.replace(Pos, ReplaceSize, asReplaceText);	// Replace symbols in position Pos for asText from asReplaceText symbols.
			Pos = asText.find(asFindText);						// Again get position for replace text.
		}

		return asText;	// Return replaced text.
	}

    // Find last of 'findText' in 'text' and put founded text after it in 'copyText'.
    // Return 'copyText'.
	std::string FindLastNCopyAfter(std::string asText, std::string asFindText)
	{
		if (asFindText == "")	// Return when got null string.
			return "";

		std::string::size_type Pos = asText.find_last_of(asFindText);	// Get last symbol in position for replace text.

		std::string asCopyText;	// This text we wanna get.
		std::copy(asText.begin() + Pos, asText.end(), std::back_inserter(asCopyText));	// Copy in all symbols from Pos to end of asText.

		return asCopyText;		// Return copied text.
	}

    // Find last of 'findText' in 'text' and put founded text before it in 'copyText'.
    // Return 'copyText'.
	std::string FindLastNCopy(std::string asText, std::string asFindText)
	{
		if(asFindText == "")	// Return when got null string.
			return "";

		std::string::size_type  Pos = asText.find_last_of(asFindText);	// Get start position from the end for replace text.

		std::string asCopyText;	// This text we wanna get.
		std::copy(asText.begin(), asText.begin() + Pos, std::back_inserter(asCopyText));	//  Copy in all symbols from begin  to Pos of a text.

		return asCopyText;	// Return copied text.
	}

	// Find 'findText' in 'text' and return true if found.
	// Return found or not found bool.
	bool Find(std::string asText, std::string asFindText)
	{
		if(asFindText == "")	// Return when got null string.
			return false;

		std::string::size_type Pos = asText.find(asFindText);		// Get start position of a find text.

		// If got return true.
		if(Pos < asText.size() && Pos != std::string::npos)
			return true;

		return false;
	}

// This class helps to write char 'fill' 'width' times in ostream.
struct cFillandWString
{
	cFillandWString(char f, int w):fill(f), width(w){}

	char fill;
	int width;

    // An outside class function, which have access to private and protected variables, but implemented inside class because of errors.
    // Can be defined in .cpp, which we don't have.
	friend std::ostream& operator<<(std::ostream& o, const cFillandWString& a)
	{
		o.fill(a.fill);
		o.width(a.width);
		return o;
    }
};


#endif /* COMMONSTRINGFUNCTIONS_H_ */
