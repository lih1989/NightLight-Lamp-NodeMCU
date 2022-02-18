<template>
  <div class="flex h-screen bg-white">
    <div class="w-full max-w-xs m-auto bg-gray-800 rounded p-5">
      <!-- header -->
      <header>
        <img class="w-20 mx-auto mb-5" src="@/assets/arduino-logo.png" />
      </header>
      <!-- form -->
      <form>
        <div class="flex flex-wrap items-stretch w-full mb-4 relative">
          <div class="bg-gray-200 flex -mr-px rounded-l">
            <span
              class="flex items-center leading-normal bg-grey-lighter rounded rounded-r-none border border-r-0 border-grey-light px-3 whitespace-no-wrap text-grey-dark text-sm">ws://</span>
          </div>
          <input type="text"
                 class="flex-shrink flex-grow flex-auto leading-normal w-px flex-1 border h-10 border-grey-light px-3 relative focus:outline-none focus:bg-white focus:text-gray-900">
        </div>
        <div>
          <button class="w-full font-normal py-2 px-4 rounded bg-green-500 text-white">
            Connect
          </button>
          <button class="w-full font-normal py-2 px-4 rounded bg-blue-500 hover:bg-blue-700 text-white" disabled>
            Connect 2
          </button>
        </div>
      </form>
      <!-- footer -->
      <footer class="pt-5">
        <a class="text-white hover:text-grey-dark text-sm float-right" href="#">Help?</a>
      </footer>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'

export default defineComponent({
  props: {
    msg: String
  }
})
</script>
