#pragma once
#include <vector>

/// <summary>
/// A collection of handy functions
/// </summary>
namespace Tools
{
	/// <summary>
	/// Finds anything in a vector independent of the type of the vector
	/// </summary>
	/// <typeparam name="T"> is the type of the vector and the item being searched for</typeparam>
	/// <param name="list"> the list is a list where the item possibly could be in</param>
	/// <param name="item"> is the instance being looked for</param>
	/// <returns> the position in the list, if -1 is being returned that means the instance is not present in the vector</returns>
	template<typename T> static int FindInList(std::vector<T>& list, const T& item)
	{
		for (int i = 0; i < list.size(); i++)
		{
			if (&list[i] == &item) return i;
		}
		return -1;
	}

	/// <summary>
	/// Deletes anything in a vector independent of the type of the vector
	/// </summary>
	/// <typeparam name="T">is the type of the vector and the item being deleted</typeparam>
	/// <param name="list"> the list is a list where the item possibly could be in</param>
	/// <param name="item"> the instance to be deleted</param>
	template<typename T> static void DeleteFromList(std::vector<T>& list, const T& item)
	{
		int placeInList = FindInList<T>(list, item);
		if (placeInList == -1) return;
		if (list.size() > 0)
			list.erase(list.begin() + placeInList);
	}

	/// <summary>
	/// moves an element from one vector into a new vector
	/// </summary>
	/// <typeparam name="T"> is the type of the vector</typeparam>
	/// <param name="oldList"> is the old list you want to copy the element from</param>
	/// <param name="newList"> is the new list you want to put the element into</param>
	/// <param name="item"> is the instance you want to move over</param>
	template<typename T> static void MoveToList(std::vector<T>& oldList, std::vector<T>& newList, const T& item)
	{
		int placeInOldList = FindInList<T>(oldList, item);
		if (placeInOldList == -1) return;
		if (oldList.size() > 0)
			newList.push_back(std::move(oldList.at(placeInOldList)));
	}

	/// <summary>
	/// Converts an array to a vector
	/// </summary>
	/// <typeparam name="T"> the type being used </typeparam>
	/// <typeparam name="S"> the size of the array</typeparam>
	/// <param name="arr"> the array that should be converted to a vector</param>
	/// <returns> a new vector containing all of the values from the array</returns>
	template<typename T, int S> static std::vector<T> ArrayToVector(T(&arr)[S])
	{
		std::vector<T> vec;

		for (int i = 0; i < S; i++)
		{
			vec.at(i) = arr.at(i);
		}

		return vec;
	}

	/// <summary>
	/// Converts a vector to and array
	/// </summary>
	/// <typeparam name="T"> the type being used</typeparam>
	/// <typeparam name="S"> the size of the array</typeparam>
	/// <param name="vec"> the vector that should be converted to an array</param>
	/// <returns> a new array containing all of the values from the vector</returns>
	template<typename T, int S> static std::array<T, S> VectorToArray(std::vector<T> vec)
	{
		T arr[S];

		for (int i = 0; i < S; i++)
		{
			arr.at(i) = vec.at(i);
		}
		return arr;
	}
	/// <summary>
	/// clamps the value between the min and max range
	/// </summary>
	/// <param name="value:">the value you want to clamp</param>
	/// <param name="min:">the minimum</param>
	/// <param name="max:">the maximum</param>
	/// <returns>the clamped value</returns>
	static float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
}