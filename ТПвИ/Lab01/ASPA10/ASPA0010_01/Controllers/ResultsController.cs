using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using ResultsAuthenticate;
using ResultsCollection;
using System.ComponentModel.DataAnnotations;

namespace ASPA0010_1.Controllers
{
	[ApiController]
	[Route("api/[controller]")]
	public class ResultsController : ControllerBase
	{
		private readonly IResultsService _resultsService;
		private readonly IAuthenticateService _auth;
		private readonly UserManager<IdentityUser> _userManager;
		private readonly SignInManager<IdentityUser> _signInManager;

		public ResultsController(IResultsService resultsService, IAuthenticateService auth,
			UserManager<IdentityUser> userManager,
			SignInManager<IdentityUser> signInManager)
		{
			_resultsService = resultsService;
			_userManager = userManager;
			_signInManager = signInManager;
			_auth = auth;
		}

		[HttpGet]
		[Authorize(Roles = "READER")]
		public async Task<IActionResult> GetAllAsync()
		{
			var results = await _resultsService.GetAllAsync();

			return Ok(results);
		}

		[HttpGet("{k:int}")]
		[Authorize(Roles = "READER")]
		public async Task<IActionResult> GetByKeyAsync(int key)
		{
			var result = await _resultsService.GetAsync(key);

			if (result == null) return NotFound();
			else return Ok(new { key, result });
		}

		[HttpPost]
		[Authorize(Roles = "WRITER")]
		public async Task<IActionResult> AddAsync([FromBody] string value)
		{
			if (string.IsNullOrWhiteSpace(value)) return BadRequest("Value should not be empty");

			var key = await _resultsService.AddAsync(value);
			return Ok(key);
		}

		[HttpPut("{k:int}")]
		[Authorize(Roles = "WRITER")]
		public async Task<IActionResult> UpdateAsync(int k, [FromBody] string value)
		{
			if (string.IsNullOrWhiteSpace(value)) return BadRequest("Value should not be empty");


			var updated = await _resultsService.UpdateAsync(k, value);
			if (!updated) return NotFound();

			else return Ok(new {k, value});

		}

		[HttpDelete("{k:int}")]
		[Authorize(Roles = "WRITER")]
		public async Task<IActionResult> DeleteAsync(int k)
		{
			var deleted = await _resultsService.DeleteAsync(k);

			if (!deleted) return NotFound();
			else return Ok();
		}


		[HttpPost("SignIn")]
		[AllowAnonymous]
		public async Task<IActionResult> SignIn([FromBody] LoginModel model)
		{
			if (!ModelState.IsValid) return BadRequest(ModelState);

			var user = await _auth.FindByNameAsync(model.Login);
			if (user == null) return NotFound("User not found.");

			var result = await _auth.SignInAsync(model.Login, model.Password);
			if (result.Succeeded) return Ok("Sign in successful.");

			return BadRequest("Invalid login attempt.");
		}

		[HttpGet("SignOut")]
		[Authorize]
		public async Task<IActionResult> SignOut()
		{
			await _auth.SignOutAsync();
			return Ok("Sign out successful.");
		}

		public class LoginModel
		{
			[Required]
			public string Login { get; set; } = null!;

			[Required]
			public string Password { get; set; } = null!;
		}

	}
}
