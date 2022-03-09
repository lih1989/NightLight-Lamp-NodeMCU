interface socketEvent {
  data: string;
  mutation: string;
  namespace: string;
  action: string;
}

// Кастомный обработчик входящий хообщений
const customPassToStoreHandler = function (eventName: string, event: socketEvent) {
  // If the beginning of the event name is not SOCKET_ then terminate the function
  if (!eventName.startsWith('SOCKET_')) {
    return
  }
  let method = 'commit'
  // Turn the letter of the event name to uppercase
  let target = eventName.toUpperCase()
  // Message content
  let msg = event
  // data exists and the data is in json format
  if (this.format === 'json' && event.data) {
    // Convert data from json string to json object
    try {
      msg = JSON.parse(event.data)
      // Determine whether msg is synchronous or asynchronous
      if (msg.mutation) {
        target = [msg.namespace || '', msg.mutation].filter((e: string) => !!e).join('/')
      } else if (msg.action) {
        method = 'dispatch'
        target = [msg.namespace || '', msg.action].filter((e: string) => !!e).join('/')
      }
    } catch (e) {
      console.error(eventName, target, e.message, msg)
      return this.store[method](target, { error: e.message || 'Ошибка обработки сообщения' })
    }
  }
  if (this.mutations) {
    target = this.mutations[target] || target
  }
  // Trigger methods in storm
  this.store[method](target, msg)
}

// Преобразую число(миллисекунды) в читаемый формат Часы:Минуты:Секунды
const numberToHhMmSs = function (timestamp = 9462) {
  const hours = Math.floor(timestamp / 60 / 60)
  const minutes = Math.floor(timestamp / 60) - (hours * 60)
  const seconds = timestamp % 60

  return [
    (hours > 9 ? '' : '0') + hours + 'ч ',
    (minutes > 9 ? '' : '0') + minutes + 'м ',
    (seconds > 9 ? '' : '0') + seconds + 'с '
  ].join('')
  // return hours + ':' + minutes + ':' + seconds
}
export {
  customPassToStoreHandler,
  numberToHhMmSs
}
