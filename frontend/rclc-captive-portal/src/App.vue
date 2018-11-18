<template>
  <div id="app">
    <h1>RC Lego Car Configuration</h1>
    <div v-if="status == ''">
      <p v-for="field in Object.keys(config)" :key="field">
        <label :for="field">{{ field }}</label>
        <input :name="field" v-model="config[field]">
      </p>
      <p>
        <button @click="save">Save</button>
      </p>
    </div>
    <div v-else>
      <h4>Trying to connect to {{ config.ssid }}...</h4>
    </div>
  </div>
</template>

<script>
export default {
  data () {
    return {
      config: {},
      status: "",
    }
  },
  mounted () {
    fetch("/api/config")
      .then(response => response.text())
      .then(data => {
        this.config = data
          .split("\n")
          .map(entry => {
            const idx = entry.indexOf('=')
            return idx > 1 ? { [entry.substring(0, idx)]: entry.substring(idx + 1) } : {}
          })
          .reduce((acc, field) => Object.assign({}, acc, field ), {});
      })
  },
  methods: {
    save () {
      this.status = "Saving values and connecting to wifi...";
      fetch("/api/config", {
        method: "POST",
        body: Object.keys(this.config).map(field => `${field}=${this.config[field]}`).join('\n'),
        headers: {
          "Content-Type": "application/text",
        },
      })
    }
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
