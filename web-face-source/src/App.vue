<template>
  <div class="flex flex-col h-screen overflow-hidden">
    <!--  HEADER  -->
    <header class="w-full">
      <HeaderBlock/>
    </header>
    <!--  CONTENT  -->
    <main class="flex-1 overflow-auto">
      <router-view/>
    </main>
    <!--  FOOTER  -->
    <footer class="w-full bg-gray-800">
      <div class="px-2 py-1 text-center text-white font-normal">
        Made by with Vue.js
      </div>
    </footer>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import HeaderBlock from '@/components/HeaderBlock.vue'

export default defineComponent({
  components: { HeaderBlock }
})
</script>
