using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using ResultsAuthenticate;
using ResultsCollection;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContext<ApplicationDbContext>(options =>
	options.UseSqlServer(builder.Configuration.GetConnectionString("DefaultConnection"),
						 b => b.MigrationsAssembly("ASPA0010_01")));

builder.Services.AddIdentity<IdentityUser, IdentityRole>(o =>
{
	o.SignIn.RequireConfirmedPhoneNumber = false;
	o.SignIn.RequireConfirmedEmail = false;
	o.Password.RequiredUniqueChars = 1;
	o.Password.RequireNonAlphanumeric = false;
	o.Password.RequireDigit = false;
	o.Password.RequireLowercase = false;
	o.Password.RequireUppercase = false;
	o.Password.RequiredLength = 4;
})
.AddEntityFrameworkStores<ApplicationDbContext>()
.AddDefaultTokenProviders();



builder.Services.AddScoped<IAuthenticateService, AuthenticateService>();
builder.Services.AddTransient<IResultsService>(sp =>
{
	var path = Path.Combine(builder.Environment.ContentRootPath, "results.json");
	return new ResultsService(path);
});


builder.Services.AddAuthentication(CookieAuthenticationDefaults.AuthenticationScheme)
	.AddCookie(options =>
	{
		options.LoginPath = "/api/Results/SignIn";
		options.LogoutPath = "/api/Results/SignOut";
	});

builder.Services.AddAuthorization(options =>
{
	options.AddPolicy("RequireReader", policy => policy.RequireRole("READER"));
	options.AddPolicy("RequireWriter", policy => policy.RequireRole("WRITER"));
});

builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

app.UseSwagger();
app.UseSwaggerUI();

app.UseHttpsRedirection();

app.UseAuthentication();
app.UseAuthorization();

using (var scope = app.Services.CreateScope())
{
	var services = scope.ServiceProvider;
	var db = services.GetRequiredService<ApplicationDbContext>();
	db.Database.Migrate();   

	var roleManager = services.GetRequiredService<RoleManager<IdentityRole>>();
	var userManager = services.GetRequiredService<UserManager<IdentityUser>>();

	string[] roles = new[] { "READER", "WRITER" };
	foreach (var role in roles)
	{
		if (!await roleManager.RoleExistsAsync(role))
			await roleManager.CreateAsync(new IdentityRole(role));
	}

	var testUser = await userManager.FindByNameAsync("admin");
	if (testUser == null)
	{
		testUser = new IdentityUser { UserName = "admin", Email = "admin@example.com", EmailConfirmed = true };
		await userManager.CreateAsync(testUser, "Admin123!");
		await userManager.AddToRolesAsync(testUser, roles);
	}
}

app.MapControllers();

app.Run();


