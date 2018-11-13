<template>
  <div id="app">
    <h1>RC Lego Car Configuration</h1>
    <p v-for="field in Object.keys(config)" :key="field">
      <label :for="field">{{ field }}</label>
      <input :name="field" v-model="config[field]">
    </p>
    <p>
      <button @click="save">Save</button>
    </p>
    <h3>{{ status }}</h3>
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
      .then(response => response.json())
      .then(data => { this.config = data })
  },
  methods: {
    save () {
      this.status = "Saving values and connecting to wifi...";
      fetch("/api/config", {
        method: "POST",
        body: JSON.stringify(this.config),
        headers: {
          "Content-Type": "application/json",
        },
      })
      setTimeout(() => {
        fetch("/api/connected")
          .then(response => response.text()) 
          .then(status => { this.status = status });
      }, 5000);
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
