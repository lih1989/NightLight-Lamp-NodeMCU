<template>
  <div>
    <div class="p-3">
<!--      <label for="toggle-example-checked" class="flex items-center cursor-pointer relative">-->
<!--        <input type="checkbox" id="toggle-example-checked" class="sr-only" :disabled="false">-->
<!--        <div class="toggle-bg bg-gray-200 border-2 border-gray-200 h-6 w-11 rounded-full"></div>-->
<!--        <span class="ml-3 text-gray-900 text-sm font-medium">Toggle me</span>-->
<!--      </label>-->
      <label for="toggle-example-checked2" class="flex items-center cursor-pointer relative">
        <input type="checkbox" id="toggle-example-checked2" class="sr-only"
               :checked="currConfigState.status"
               :value="currConfigState.status"
               :disabled="false"
               @input="configStateValues = { key: 'status', value: $event }">
        <div class="toggle-bg bg-gray-200 border-2 border-gray-200 h-6 w-11 rounded-full"></div>
        <span class="ml-3 text-gray-900 text-sm font-medium">Toggle me ({{configStateValues.status}})</span>
      </label>
<!--    </div>-->
<!--    <div class="relative border rounded pa-2">-->
<!--          <span class="absolute inset-y-0 left-0 flex items-center pl-2 text-gray-400">-->
<!--            ws://-->
<!--          </span>-->
<!--      <input type="text" pattern="^([0-9a-fA-F]{4}:){7}[0-9a-fA-F]{4}$"-->
<!--             class="py-2 text-sm text-white pl-12 focus:outline-none focus:bg-white focus:text-gray-900"-->
<!--             placeholder="xxx.xxx.xxx.xxx" autocomplete="on">-->
<!--    </div>-->
<!--    <div class="relative p-4 border border-grey-lighter w-1/2">-->
<!--      <div class="flex flex-wrap items-stretch w-full mb-4 relative">-->
<!--        <div class="bg-gray-200 flex -mr-px rounded-l">-->
<!--          <span-->
<!--            class="flex items-center leading-normal bg-grey-lighter rounded rounded-r-none border border-r-0 border-grey-light px-3 whitespace-no-wrap text-grey-dark text-sm">ws://</span>-->
<!--        </div>-->
<!--        <input type="text"-->
<!--               class="flex-shrink flex-grow flex-auto leading-normal w-px flex-1 border h-10 border-grey-light px-3 relative focus:outline-none focus:bg-white focus:text-gray-900">-->
<!--        <div class="bg-gray-200 flex -mr-px rounded-r">-->
<!--          <button class="font-normal py-2 px-4 rounded-none bg-green-500 text-white">-->
<!--            Connected-->
<!--          </button>-->
<!--          <button class="font-normal py-2 px-4 rounded-none bg-blue-500 hover:bg-blue-700 text-white">-->
<!--            connectionProcess-->
<!--          </button>-->
<!--        </div>-->
<!--      </div>-->
    </div>
    TODO RANGE
    <div style="max-width: 300px; background-color: #cccccc">
      <div class="relative pt-1">
        <label for="customRange3" class="form-label">Example range {{int}} | {{ readableTime }}</label>
        <input
          type="range"
          class="
      form-range
      w-full
      h-6
      p-0
      focus:outline-none focus:ring-0 focus:shadow-none
    "
          min="0"
          max="3600"
          step="1"
          :value="currConfigState.motionSensorSeconds"
          @input="changeTime"
          id="customRange3"
        />
      </div>
    </div>
    currConfigState:
    <pre>
      {{ currConfigState }}
    </pre>
    configStateValues:
    <pre>
      {{ configStateValues }}
    </pre>
    <button @click="clickButton" class="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded-full">
      Test Send
    </button>
    <button class="select-none inline-flex items-center justify-center w-10 h-10 mr-2 text-indigo-100 transition-colors duration-150 bg-indigo-700 rounded-full focus:shadow-outline hover:bg-indigo-800">
      <svg class="w-4 h-4 fill-current" viewBox="0 0 20 20">
        <path
          d="M3.172 5.172a4 4 0 015.656 0L10 6.343l1.172-1.171a4 4 0 115.656 5.656L10 17.657l-6.828-6.829a4 4 0 010-5.656z"
          clip-rule="evenodd" fill-rule="evenodd"></path>
      </svg>
    </button>

  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { mapGetters, mapState } from 'vuex'
import { numberToHhMmSs } from '@/Helper'
import lodashDebounce from 'lodash/debounce'

export default defineComponent({
  name: 'ManualControlMode',
  data () {
    return {
      bool: false,
      int: 5
    }
  },
  methods: {
    clickButton () {
      // Call the send method to send data as a string
      console.warn(this.$socket)
      // this.$socket.send('some data')
      // If fomat is configured as json, you can call the send Obj method to send data
      this.$socket.sendObj({ volume: Math.floor(Math.random() * 9) })
    },
    changeTime: lodashDebounce(function (value: any) {
      console.log('changeTime', value, this.readableTime)
      this.configStateValues = { key: 'motionSensorSeconds', value: value }
      // this.$socket.sendObj({ motionSensorSeconds:  * 1000 })
    }, 300)
  },
  computed: {
    ...mapState({
      configState: 'configState'
    }),
    ...mapGetters({
      currConfigState: 'currConfigState'
    }),
    configStateValues: {
      get () {
        return this.currConfigState || {}
      },
      set (event) {
        console.warn('configStateValues set', event)
        let resultValue = null
        switch (event.key) {
          case 'status': {
            resultValue = event.value.target.checked
            break
          }
          case 'motionSensorSeconds': {
            resultValue = isNaN(Number(event.value.target.value)) ? 0 : Number(event.value.target.value)
            break
          }
          default:
            break
        }
        // Отправка по сокету
        console.warn('------------', { [event.key]: resultValue })
        this.$socket.sendObj({ [event.key]: resultValue })
      }
    },
    readableTime () {
      return numberToHhMmSs(this.int)
    }
  }
})
</script>
<style lang="scss">
//.toggle-bg:after {
//  content: '';
//  @apply absolute top-0.5 left-0.5 bg-white border border-gray-300 rounded-full h-5 w-5 transition shadow-sm;
//}
//
//input:checked + .toggle-bg:after {
//  transform: translateX(100%);
//  @apply border-white;
//}
//
//input:checked + .toggle-bg {
//  @apply bg-blue-600 border-blue-600;
//}
//input:disabled + .toggle-bg {
//  @apply bg-gray-300 border-gray-300;
//}
</style>
