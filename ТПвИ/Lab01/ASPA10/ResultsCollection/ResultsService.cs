using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using System.Threading.Tasks;

namespace ResultsCollection
{
	public class ResultsService : IResultsService
	{
		private readonly string _filePath;
		private readonly object _lock = new();
		private Dictionary<int, string> _results;
		private int _nextKey;

		public ResultsService(string filePath)
		{
			_filePath = filePath;
			_results = LoadFromFile();
			_nextKey = _results.Count > 0 ?
					   _results.Keys.Max() + 1 : 1;
		}

		public Task<Dictionary<int, string>> GetAllAsync()
		{
			lock (_lock)
			{
				return Task.FromResult(new Dictionary<int, string>(_results));
			}
		}

		public Task<string?> GetAsync(int key)
		{
			lock (_lock)
			{
				return Task.FromResult(_results.TryGetValue(key, out var value) ? value : null);
			}
		}

		public Task<int> AddAsync(string value)
		{
			lock (_lock)
			{
				int key = _nextKey++;
				_results[key] = value;
				SaveToFile();
				return Task.FromResult(key);
			}
		}

		public Task<bool> UpdateAsync(int key, string value)
		{
			lock (_lock)
			{
				if (!_results.ContainsKey(key))
					return Task.FromResult(false);

				_results[key] = value;
				SaveToFile();
				return Task.FromResult(true);
			}
		}

		public Task<bool> DeleteAsync(int key)
		{
			lock (_lock)
			{
				if (!_results.Remove(key))
					return Task.FromResult(false);

				SaveToFile();
				return Task.FromResult(true);
			}
		}

		private Dictionary<int, string> LoadFromFile()
		{
			if (!File.Exists(_filePath))
				return new Dictionary<int, string>();

			var json = File.ReadAllText(_filePath);
			return string.IsNullOrWhiteSpace(json)
				? new Dictionary<int, string>()
				: JsonSerializer.Deserialize<Dictionary<int, string>>(json)
				  ?? new Dictionary<int, string>();
		}

		private void SaveToFile()
		{
			var json = JsonSerializer.Serialize(_results, new JsonSerializerOptions { WriteIndented = true });
			File.WriteAllText(_filePath, json);
		}
	}
}
