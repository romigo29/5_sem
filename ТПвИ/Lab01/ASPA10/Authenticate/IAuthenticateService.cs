using System.Security.Claims;
using System.Threading.Tasks;

namespace ResultsAuthenticate
{
	using Microsoft.AspNetCore.Identity;
	using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
	using Microsoft.EntityFrameworkCore;


	public interface IAuthenticateService
	{
		Task<SignInResult> SignInAsync(string login, string password);
		Task SignOutAsync();

		Task<IdentityUser?> FindByNameAsync(string login);
	}
}