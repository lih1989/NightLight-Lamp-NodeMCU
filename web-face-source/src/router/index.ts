import { createRouter, createWebHashHistory } from 'vue-router'
import AuthPage from '@/components/AuthPage.vue'
import HelloTaleWind from '@/components/HelloTailWind.vue'
import ManualControlMode from '@/components/ManualControlMode.vue'

export default createRouter({
  history: createWebHashHistory(),
  routes: [
    { name: 'Главная', path: '/', component: AuthPage, props: { msg: 'TODO' } },
    { name: 'Tailwind', path: '/tailwind', component: HelloTaleWind },
    { name: 'Manual', path: '/manual-control-mode', component: ManualControlMode }
  ]
})
