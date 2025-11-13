using ASPA011_01.Models;
using ASPA011_01.Services;
using ASPA011_1.Services;
using Microsoft.Extensions.Logging;

var builder = WebApplication.CreateBuilder(args);

// Настройки очереди
builder.Services.Configure<QueueSettings>(builder.Configuration.GetSection("QueueSettings"));

// Регистрация сервиса
builder.Services.AddSingleton<IChannelManager, ChannelManager>();

builder.Services.AddControllers()
	.AddJsonOptions(options =>
	{
		options.JsonSerializerOptions.Converters.Add(new System.Text.Json.Serialization.JsonStringEnumConverter());
	});

// ----------------- ЛОГГИРОВАНИЕ -----------------
builder.Logging.ClearProviders();
builder.Logging.AddConsole(); // Warning, Error, Critical → консоль
builder.Logging.AddDebug();   // Debug → окно Visual Studio
builder.Logging.AddProvider(new FileLoggerProvider("logs/log.txt")); // Trace, Debug, Information → файл


// Настройка уровней логирования
builder.Logging.AddFilter("Microsoft", LogLevel.Warning);
builder.Logging.AddFilter("System", LogLevel.Warning);
builder.Logging.AddFilter("ASPA011_01", LogLevel.Trace); // все сообщения для нашего приложения

var app = builder.Build();

// Для Development включаем трассировку
if (app.Environment.IsDevelopment())
{
	app.UseDeveloperExceptionPage();
}

// Маршрутизация
app.MapControllers();

// Лог запуска приложения
var logger = app.Services.GetRequiredService<ILogger<Program>>();
logger.LogInformation("Приложение ASPA011_01 запущено");

app.Run();
