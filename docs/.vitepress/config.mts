import { defineConfig } from 'vitepress'
import llmstxt from 'vitepress-plugin-llms'
export default defineConfig({
  title: "QtNativeUI",
  description: "现代化的 Qt UI 库",
  rewrites: {
    'zh/:rest*': ':rest*'
  },
  lastUpdated: true,
  cleanUrls: true,
  metaChunk: true,
  locales: {
    root: {
      label: '简体中文',
      lang: 'zh-CN',
      dir: 'ltr',
      link: '/zh/'
    },
    // en: {
    //   label: 'English',
    //   lang: 'en-US',
    //   link: '/en/'
    // }
  },
  vite: {
    plugins: [
      llmstxt(
        {
          workDir: 'zh',
          ignoreFiles: ['index.md']
        }
      )
    ],
  }
})
