import { createApp } from 'vue'
import App from './App.vue'
import store from './store'
import router from './router'
import { customPassToStoreHandler } from './Helper'

import './styles/app.css'
import 'flowbite'

import VueNativeSock from 'vue-native-websocket-vue3'

const appInstance = createApp(App)

const rootComponent = appInstance
  .use(router)
  .use(store)
  .use(VueNativeSock, `ws://${location.hostname}:1181`, {
    store: store,
    format: 'json',
    // "connectManually": true,
    reconnection: true,
    reconnectionAttempts: 5,
    reconnectionDelay: 3000,
    passToStoreHandler: customPassToStoreHandler
  })
  .mount('#app')

console.warn(appInstance, rootComponent)
export default appInstance
