export interface Mapped<T> {
	[key: string]: T
}

export interface StrictMapped<T> extends Mapped<T | undefined> {}
