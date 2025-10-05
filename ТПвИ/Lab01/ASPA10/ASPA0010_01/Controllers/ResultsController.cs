using Microsoft.AspNetCore.Mvc;
using ResultsCollection;

namespace ASPA0010_1.Controllers
{
	[ApiController]
	[Route("api/[controller]")]
	public class ResultsController : ControllerBase
	{
		private readonly IResultsService _resultsService;

		public ResultsController(IResultsService resultsService)
		{
			_resultsService = resultsService;
		}

		[HttpGet]
		public async Task<IActionResult> GetAllAsync()
		{
			var results = await _resultsService.GetAllAsync();

			return Ok(results);
		}

		[HttpGet("{k:int}")]
		public async Task<IActionResult> GetByKeyAsync(int key)
		{
			var result = await _resultsService.GetAsync(key);

			if (result == null) return NotFound();
			else return Ok(new { key, result });
		}

		[HttpPost]
		public async Task<IActionResult> AddAsync([FromBody] string value)
		{
			if (string.IsNullOrWhiteSpace(value)) return BadRequest("Value should not be empty");

			var key = await _resultsService.AddAsync(value);
			return CreatedAtAction(nameof(GetByKeyAsync), new { key}, new {key , value });
		}

		[HttpPut("{k:int}")]
		public async Task<IActionResult> UpdateAsync(int k, [FromBody] string value)
		{
			if (string.IsNullOrWhiteSpace(value)) return BadRequest("Value should not be empty");


			var updated = await _resultsService.UpdateAsync(k, value);
			if (!updated) return NotFound();

			else return Ok(new {k, value});

		}

		[HttpDelete("{k:int}")]
		public async Task<IActionResult> DeleteAsync(int k)
		{
			var deleted = await _resultsService.DeleteAsync(k);

			if (!deleted) return NotFound();
			else return Ok();
		}
	}
}
