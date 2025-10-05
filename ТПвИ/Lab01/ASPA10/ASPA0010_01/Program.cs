using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using ResultsAuthenticate;
using ResultsCollection;

var builder = WebApplication.CreateBuilder(args);

// Регистрируем ResultsService как Transient
builder.Services.AddTransient<IResultsService>(sp =>
	new ResultsService(Path.Combine(AppContext.BaseDirectory, "results.json")));


builder.Services.AddDbContext<ApplicationDbContext>(options =>
	options.UseSqlServer(builder.Configuration.GetConnectionString("DefaultConnection")));

builder.Services.AddIdentity<IdentityUser, IdentityRole>()
	.AddEntityFrameworkStores<ApplicationDbContext>()
	.AddDefaultTokenProviders();

// Cookie-аутентификация
builder.Services.ConfigureApplicationCookie(options =>
{
	options.LoginPath = "/api/Results/SignIn";
	options.ExpireTimeSpan = TimeSpan.FromMinutes(30);
});

// HttpContextAccessor
builder.Services.AddHttpContextAccessor();

// Регистрируем AuthenticateService
builder.Services.AddScoped<IAuthenticateService, AuthenticateService>();


builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

if (app.Environment.IsDevelopment())
{
	app.UseSwagger();
	app.UseSwaggerUI();
}

app.UseAuthorization();
app.MapControllers();

app.Run();
