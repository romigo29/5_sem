using ASPA011_01.Models;
using ASPA011_01.Services;
using Microsoft.Extensions.Options;
using System.Collections.Concurrent;
using System.Threading.Channels;

namespace ASPA011_1.Services
{
	public class ChannelManager : IChannelManager
	{
		private readonly ConcurrentDictionary<Guid, ChannelWrapper> _channels = new();
		private readonly int _waitEnqueue;

		public ChannelManager(IOptions<QueueSettings> options)
		{
			_waitEnqueue = options.Value.WaitEnqueue;
		}



		public IEnumerable<ChannelInfo> GetAll() =>
			_channels.Values.Select(c => new ChannelInfo
			{
				Id = c.Id,
				Name = c.Name,
				State = c.State,
				Description = c.Description
			});

		public ChannelInfo? Get(Guid id)
		{
			if (_channels.TryGetValue(id, out var ch))
			{
				return new ChannelInfo
				{
					Id = ch.Id,
					Name = ch.Name,
					State = ch.State,
					Description = ch.Description
				};
			}
			return null;
		}

		public ChannelInfo Create(CreateChannel model)
		{
			var ch = new ChannelWrapper
			{
				Name = model.Name,
				Description = model.Description,
				State = model.State
			};
			_channels[ch.Id] = ch;

			return new ChannelInfo
			{
				Id = ch.Id,
				Name = ch.Name,
				State = ch.State,
				Description = ch.Description
			};
		}

		public bool CloseAll(string reason)
		{
			foreach (var ch in _channels.Values)
				ch.State = ChannelState.CLOSED;
			return true;
		}

		public bool CloseById(Guid id, string reason)
		{
			if (_channels.TryGetValue(id, out var ch))
			{
				ch.State = ChannelState.CLOSED;
				return true;
			}
			return false;
		}

		public bool OpenAll()
		{
			foreach (var ch in _channels.Values)
				ch.State = ChannelState.ACTIVE;
			return true;
		}

		public bool OpenById(Guid id)
		{
			if (_channels.TryGetValue(id, out var ch))
			{
				ch.State = ChannelState.ACTIVE;
				return true;
			}
			return false;
		}

		public bool DeleteAll()
		{
			_channels.Clear();
			return true;
		}

		public bool DeleteClosed()
		{
			var closedIds = _channels.Values.Where(c => c.State == ChannelState.CLOSED)
											.Select(c => c.Id)
											.ToList();
			foreach (var id in closedIds)
				_channels.TryRemove(id, out _);
			return true;
		}

		public Element? Enqueue(Guid id, string data)
		{
			if (!_channels.TryGetValue(id, out var ch))
				return null;

			try
			{
				var writeTask = ch.Queue.Writer.WriteAsync(data).AsTask();
				if (!writeTask.Wait(TimeSpan.FromSeconds(_waitEnqueue)))
				{
					// Таймаут достигнут
					return new Element
					{
						Id = id,
						Data = $"Error: WaitEnqueue timeout after {_waitEnqueue} seconds"
					};
				}

				return new Element { Id = id, Data = data };
			}
			catch (Exception)
			{
				return new Element { Id = id, Data = "Error: enqueue failed" };
			}
		}

		public Element? Dequeue(Guid id)
		{
			if (!_channels.TryGetValue(id, out var ch))
				return null;

			if (ch.Queue.Reader.TryRead(out var result))
			{
				return new Element { Id = id, Data = result };
			}

			return new Element { Id = id, Data = "Queue is empty" };
		}

		public Element? Peek(Guid id)
		{
			if (!_channels.TryGetValue(id, out var ch))
				return null;

			if (ch.Queue.Reader.TryPeek(out var result))
			{
				return new Element { Id = id, Data = result };
			}

			return new Element { Id = id, Data = "Queue is empty" };
		}
	}
}
