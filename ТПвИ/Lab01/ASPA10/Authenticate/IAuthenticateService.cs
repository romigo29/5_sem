using System.Security.Claims;
using System.Threading.Tasks;

namespace ResultsAuthenticate
{
	using Microsoft.AspNetCore.Identity;
	using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
	using Microsoft.EntityFrameworkCore;


	public interface IAuthenticateService
	{
		Task<bool> SignInAsync(string login, string password);
		Task SignOutAsync();
		ClaimsPrincipal? CurrentUser { get; }
	}
}