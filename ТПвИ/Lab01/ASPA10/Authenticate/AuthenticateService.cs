using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using System.Security.Claims;
using System.Threading.Tasks;

namespace ResultsAuthenticate
{

	public class ApplicationDbContext : IdentityDbContext<IdentityUser>
	{
		public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
			: base(options)
		{
		}

		// Если нужно, добавляй свои таблицы:
		// public DbSet<ResultItem> Results { get; set; }
	}

	public class AuthenticateService : IAuthenticateService
	{
		private readonly UserManager<IdentityUser> _userManager;
		private readonly SignInManager<IdentityUser> _signInManager;
		private readonly IHttpContextAccessor _httpContextAccessor;

		public AuthenticateService(
			UserManager<IdentityUser> userManager,
			SignInManager<IdentityUser> signInManager,
			IHttpContextAccessor httpContextAccessor)
		{
			_userManager = userManager;
			_signInManager = signInManager;
			_httpContextAccessor = httpContextAccessor;
		}

		public ClaimsPrincipal? CurrentUser => _httpContextAccessor.HttpContext?.User;

		public async Task<bool> SignInAsync(string login, string password)
		{
			var user = await _userManager.FindByNameAsync(login);
			if (user == null) return false;

			var result = await _signInManager.PasswordSignInAsync(
				user, password, isPersistent: true, lockoutOnFailure: false);

			return result.Succeeded;
		}

		public async Task SignOutAsync()
		{
			await _signInManager.SignOutAsync();
		}
	}
}
