using System.Collections.Generic;
using System.Threading.Tasks;

namespace ResultsCollection
{
	public interface IResultsService
	{
		Task<Dictionary<int, string>> GetAllAsync();
		Task<string?> GetAsync(int key);
		Task<int> AddAsync(string value);
		Task<bool> UpdateAsync(int key, string value);
		Task<bool> DeleteAsync(int key);
	}
}
