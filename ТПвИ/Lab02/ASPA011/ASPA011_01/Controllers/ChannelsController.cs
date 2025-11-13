using ASPA011_01.Models;
using ASPA011_01.Services;
using Microsoft.AspNetCore.Mvc;
using System.Text.Json;

namespace ASPA011_01.Controllers
{
	[ApiController]
	[Route("api/[controller]")]
	public class ChannelsController : ControllerBase
	{
		private readonly ILogger<ChannelsController> _logger;
		private readonly IChannelManager _channelManager;
	

		public ChannelsController(ILogger<ChannelsController> logger, IChannelManager channelManager)
		{
			_logger = logger;
			_channelManager = channelManager;
		}


		[HttpGet]
		public IActionResult GetAllChannels()
		{
			var channels = _channelManager.GetAll().ToList();
			if (!channels.Any())
			{
				_logger.LogInformation("GET /api/channels — пустой список каналов");
				return NoContent();
			}
			_logger.LogInformation("GET /api/channels — возвращено {Count} каналов", channels.Count);
			return Ok(channels);
		}


		[HttpGet("{id}")]
		public IActionResult GetChannelById(Guid id)
		{
			var channel = _channelManager.Get(id);
			if (channel == null)
			{
				_logger.LogWarning("GET /api/channels/{Id} — канал не найден", id);
				return NotFound();
			}
			_logger.LogInformation("GET /api/channels/{Id} — найден канал {Name}", id, channel.Name);
			return Ok(channel);
		}


		[HttpPost]
		public IActionResult CreateChannel([FromBody] CreateChannel model)
		{
			if (model == null)
				return BadRequest();

			var channel = _channelManager.Create(model);
			_logger.LogInformation("POST /api/channels — создан канал {Name} ({Id})", channel.Name, channel.Id);

			if (channel.State == ChannelState.ACTIVE)
				return StatusCode(201, channel);  
			else
				return StatusCode(204, channel);     
		}

		[HttpPut]
		public IActionResult UpdateChannels([FromBody] JsonElement commandModel)
		{
			try
			{
				if (!commandModel.TryGetProperty("command", out var commandProp))
					return BadRequest("Не указана команда");

				string command = commandProp.GetString() ?? string.Empty;

				switch (command.ToLower())
				{
					case "close":
						if (commandModel.TryGetProperty("id", out var idProp))
						{
							var model = JsonSerializer.Deserialize<CloseChannelById>(commandModel.GetRawText(),
								new JsonSerializerOptions { PropertyNameCaseInsensitive = true })!;
							var result = _channelManager.CloseById(model.Id, model.Reason);
							if (!result)
							{
								_logger.LogWarning("PUT /api/channels — канал {Id} не найден для close", model.Id);
								return NotFound();
							}
							_logger.LogInformation("PUT /api/channels — закрыт канал {Id}, причина: {Reason}", model.Id, model.Reason);
							return Ok(_channelManager.Get(model.Id));
						}
						else
						{
							var model = JsonSerializer.Deserialize<CloseAllChannels>(commandModel.GetRawText(),
								new JsonSerializerOptions { PropertyNameCaseInsensitive = true });
							_channelManager.CloseAll(model?.Reason ?? "No reason");
							_logger.LogInformation("PUT /api/channels — закрыты все каналы, причина: {Reason}", model?.Reason);
							return Ok(_channelManager.GetAll());
						}

					case "open":
						if (commandModel.TryGetProperty("id", out var openIdProp))
						{
							var model = JsonSerializer.Deserialize<OpenChannelById>(commandModel.GetRawText(),
								new JsonSerializerOptions { PropertyNameCaseInsensitive = true })!;
							var result = _channelManager.OpenById(model.Id);
							if (!result)
							{
								_logger.LogWarning("PUT /api/channels — канал {Id} не найден для open", model.Id);
								return NotFound();
							}
							_logger.LogInformation("PUT /api/channels — открыт канал {Id}", model.Id);
							return Ok(_channelManager.Get(model.Id));
						}
						else
						{
							_channelManager.OpenAll();
							_logger.LogInformation("PUT /api/channels — открыты все каналы");
							return Ok(_channelManager.GetAll());
						}

					default:
						return BadRequest("Неизвестная команда");
				}
			}
			catch (Exception ex)
			{
				_logger.LogError(ex, "Ошибка при обновлении каналов");
				return StatusCode(500);
			}
		}

		[HttpDelete]
		public IActionResult DeleteChannels([FromBody] JsonElement commandModel)
		{
			try
			{
				if (!commandModel.TryGetProperty("command", out var commandProp))
					return BadRequest("Не указана команда");

				string command = commandProp.GetString() ?? string.Empty;

				if (command.ToLower() != "delete")
					return BadRequest("Неизвестная команда");

				if (commandModel.TryGetProperty("state", out var stateProp) &&
					stateProp.GetString()?.ToUpper() == "CLOSED")
				{
					_channelManager.DeleteClosed();
					_logger.LogInformation("DELETE /api/channels — удалены все закрытые каналы");
				}
				else
				{
					_channelManager.DeleteAll();
					_logger.LogInformation("DELETE /api/channels — удалены все каналы");
				}

				return Ok(_channelManager.GetAll());
			}
			catch (Exception ex)
			{
				_logger.LogError(ex, "Ошибка при удалении каналов");
				return StatusCode(500);
			}
		}


		[HttpPost("/api/queue")]
		public IActionResult QueueOperation([FromBody] JsonElement commandModel)
		{
			try
			{
				if (!commandModel.TryGetProperty("command", out var commandProp))
					return BadRequest("Не указана команда");

				string command = commandProp.GetString()?.ToLower() ?? string.Empty;

				switch (command)
				{
					case "enqueue":
						var enqueueModel = JsonSerializer.Deserialize<EnqueueElement>(commandModel.GetRawText(),
							new JsonSerializerOptions { PropertyNameCaseInsensitive = true })!;
						var enqueueResult = _channelManager.Enqueue(enqueueModel.Id, enqueueModel.Data);

						if (enqueueResult == null)
						{
							_logger.LogWarning("POST /api/queue — канал {Id} не найден для enqueue", enqueueModel.Id);
							return NotFound();
						}

						if (enqueueResult.Data.StartsWith("Error: WaitEnqueue"))
							_logger.LogWarning("POST /api/queue — {Message}", enqueueResult.Data);
						else
							_logger.LogInformation("POST /api/queue — enqueue в канал {Id}: {Data}", enqueueModel.Id, enqueueModel.Data);

						return Ok(enqueueResult);

					case "dequeue":
						var dequeueModel = JsonSerializer.Deserialize<DequeueElement>(commandModel.GetRawText(),
							new JsonSerializerOptions { PropertyNameCaseInsensitive = true })!;
						var dequeueResult = _channelManager.Dequeue(dequeueModel.Id);

						if (dequeueResult == null)
						{
							_logger.LogWarning("POST /api/queue — канал {Id} не найден для dequeue", dequeueModel.Id);
							return NotFound();
						}

						_logger.LogInformation("POST /api/queue — dequeue из канала {Id}: {Data}", dequeueModel.Id, dequeueResult.Data);
						return Ok(dequeueResult);

					case "peek":
						var peekModel = JsonSerializer.Deserialize<DequeueElement>(commandModel.GetRawText(),
							new JsonSerializerOptions { PropertyNameCaseInsensitive = true })!;
						var peekResult = _channelManager.Peek(peekModel.Id);

						if (peekResult == null)
						{
							_logger.LogWarning("POST /api/queue — канал {Id} не найден для peek", peekModel.Id);
							return NotFound();
						}

						_logger.LogInformation("POST /api/queue — peek канала {Id}: {Data}", peekModel.Id, peekResult.Data);
						return Ok(peekResult);

					default:
						return BadRequest("Неизвестная команда");
				}
			}
			catch (Exception ex)
			{
				_logger.LogError(ex, "Ошибка при работе с очередью");
				return StatusCode(500);
			}
		}
	}
}
