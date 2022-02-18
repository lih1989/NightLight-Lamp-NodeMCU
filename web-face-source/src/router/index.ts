import { createRouter, createWebHashHistory } from 'vue-router'
import AuthPage from '@/components/AuthPage.vue'
import HelloTaleWind from '@/components/HelloTailWind.vue'
import ManualControlMode from '@/components/ManualControlMode.vue'

export default createRouter({
  history: createWebHashHistory(),
  routes: [
    { path: '/', component: AuthPage, props: { msg: 'TODO' } },
    { path: '/tailwind', component: HelloTaleWind },
    { path: '/manual-control-mode', component: ManualControlMode }
  ]
})
